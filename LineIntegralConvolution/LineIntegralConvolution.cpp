/*
Copyright (c) 2026, Michael Kazhdan
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the Johns Hopkins University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#include "Include/PreProcess.h"

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <random>
#include <sstream>

#include <Misha/Exceptions.h>
#include <Misha/Geometry.h>
#include <Misha/CmdLineParser.h>
#include <Misha/PlyData.h>
#include <Misha/Miscellany.h>
#include <Misha/SimplexMesh.h>
#include <Misha/OrthogonalFrame.h>

#include "Include/PLYVFFace.h"

using namespace MishaK;

static const unsigned int K = 2;
static const unsigned int Dim = 3;

CmdLineParameter< std::string >
	In( "in" ) ,
	Out( "out" );

CmdLineParameter< unsigned int >
	RandomSeed( "seed" , 0 ) ,
	Degree( "degree" , 2 ) ,
	Subdivide( "sub" , 0 ) ,
	SoftMaxP( "p" , 8 );

CmdLineParameter< double >
	SharpeningWeight( "sWeight" , 1e-5 ) ,
	DiffusionStepSize( "dStep" , 2.e-3 ) ,
	AnisotropyExponent( "aExp" , 8. ) ,
	AnisotropyScale( "aScale" , 1e8 ) ,
	SharpeningScale( "sScale" , 1024 );

CmdLineReadable
	GrayScale( "gray" ) ,
	Verbose( "verbose" );

std::vector< CmdLineReadable * > params =
{
	&In ,
	&Out ,
	&AnisotropyScale ,
	&AnisotropyExponent ,
	&Degree ,
	&DiffusionStepSize ,
	&SharpeningWeight ,
	&SharpeningScale ,
	&SoftMaxP ,
	&RandomSeed ,
	&GrayScale ,
	&Subdivide ,
	&Verbose
};


void
ShowUsage
(
	const char * ex
)
{
	printf( "Usage %s:\n" , ex );
	printf( "\t --%s <input mesh>\n" , In.name.c_str() );
	printf( "\t[--%s <output mesh>]\n" , Out.name.c_str() );
	printf( "\t[--%s <element degree>=%d]\n" , Degree.name.c_str() , Degree.value );
	printf( "\t[--%s <diffusion step size>=%e]\n" , DiffusionStepSize.name.c_str() , DiffusionStepSize.value );
	printf( "\t[--%s <anisotropy exponent>=%f]\n" , AnisotropyExponent.name.c_str() , AnisotropyExponent.value );
	printf( "\t[--%s <anisotropy scale>=%e]\n" , AnisotropyScale.name.c_str() , AnisotropyScale.value );
	printf( "\t[--%s <sharpening step size>=%e]\n" , SharpeningWeight.name.c_str() , SharpeningWeight.value );
	printf( "\t[--%s <sharpening scale>=%e]\n" , SharpeningScale.name.c_str() , SharpeningScale.value );
	printf( "\t[--%s <p-norm>=%d]\n" , SoftMaxP.name.c_str() , SoftMaxP.value );
	printf( "\t[--%s <planar 1-to-4 subdivision iterations>=%d]\n" , Subdivide.name.c_str() , Subdivide.value );
	printf( "\t[--%s <random seed>=%d]\n" , RandomSeed.name.c_str() , RandomSeed.value );
	printf( "\t[--%s]\n" , GrayScale.name.c_str() );
	printf( "\t[--%s]\n" , Verbose.name.c_str() );
}

void
PlanarSubdivide
(
	std::vector< Point< double , Dim > > & vertices ,
	std::vector< SimplexIndex< K > > & simplices ,
	std::vector< Point< double , Dim > > & vectorField
)
{
	std::vector< SimplexIndex< K > > _simplices;
	std::map< std::pair< unsigned int , unsigned int > , unsigned int > eMap;

	auto EdgeKey = []( unsigned int v1 , unsigned int v2 ){ return v1<v2 ? std::make_pair( v1 , v2 ) : std::make_pair( v2 , v1 ); };

	bool vertexVectors = vectorField.size()==vertices.size();
	bool simplexVectors = vectorField.size()==simplices.size();

	for( unsigned int i=0 ; i<simplices.size() ; i++ )
	{
		for( unsigned int k=0 ; k<=K ; k++ )
		{
			std::pair< unsigned int , unsigned int > eKey = EdgeKey( simplices[i][k] , simplices[i][(k+1)%(K+1)] );
			if( eMap.find( eKey )==eMap.end() )
			{
				eMap[eKey] = static_cast< unsigned int >( vertices.size() );
				vertices.push_back( ( vertices[ simplices[i][k] ] + vertices[ simplices[i][(k+1)%(K+1)] ] ) / 2 );
				if( vertexVectors ) vectorField.push_back( ( vectorField[ simplices[i][k] ] + vectorField[ simplices[i][(k+1)%(K+1)] ] ) / 2 );
			}
		}
	}

	for( unsigned int i=simplices.size() ; i!=0 ; i-- )
	{
		unsigned int idx = i-1;
		unsigned int edges[K+1];
		for( unsigned int k=0 ; k<=K ; k++ ) edges[k] = eMap[ EdgeKey( simplices[idx][(k+1)%(K+1)] , simplices[idx][(k+2)%(K+1)] ) ];
		for( unsigned int k=0 ; k<=K ; k++ )
		{
			simplices.emplace_back( simplices[idx][k] , static_cast< unsigned int >( edges[(k+2)%(K+1)] ) , static_cast< unsigned int >( edges[(k+1)%(K+1)] ) );
			if( simplexVectors ) vectorField.push_back( vectorField[idx] );
		}
		simplices[idx] = SimplexIndex< K >( edges[0] , edges[1] , edges[2] );
	}
}


std::vector< SquareMatrix< double , Dim > >
LICMetric
(
	const std::vector< Point< double , Dim > > & vertices ,
	const std::vector< SimplexIndex< K > > & simplices ,
	const std::vector< Point< double , Dim > > & vectorField ,
	unsigned int P
)
{
	std::vector< SquareMatrix< double , Dim > > metric( simplices.size() );

	// Normalizing the vector field magnitude
	double softMax = 0;
	{
		for( unsigned int i=0 ; i<simplices.size() ; i++ ) softMax += pow( Point< double , 3 >::Length( vectorField[i] ) , P );
		softMax = pow( softMax / simplices.size() , 1./P );
	}
	if( Verbose.set ) std::cout << "Soft max: " << softMax << std::endl;

	auto GetSimplex = [&]( size_t s )
		{
			Simplex< double , Dim , K > simplex;
			for( unsigned int k=0 ; k<=K ; k++ ) simplex[k] = vertices[ simplices[s][k] ];
			return simplex;
		};

	ThreadPool::ParallelFor
		(
			0 , simplices.size() ,
			[&]( size_t s )
			{
				double scale = Point< double, Dim >::Length( vectorField[s] );
				if( !scale ) metric[s] = SquareMatrix< double , K >::Identity();
				else
				{
					Simplex< double , Dim , K > simplex = GetSimplex( s );
					Point< double , Dim > p[] = { simplex.normal() , vectorField[s] };
					OrthogonalFrame< Dim > frame( p , 2 );
					scale /= softMax;

					// Compute the change of basis from edges to the orthgonal tangent frame basis
					SquareMatrix< double , K > X;
					for( unsigned int j=0 ; j<K ; j++ ) for( unsigned int k=0 ; k<K ; k++ ) X(j,k) = Point< double , Dim >::Dot( frame[k+1] , simplex[j+1]-simplex[0] );

					// Compute the matrix performing the anisotropic scaling
					// The diagonal should look like { 1 , f(s) } where f(s) = 1 when s=0 and increases monotonically with s
					// =>  f(s) = a + s^b
					SquareMatrix< double , 2 > D;
					D(0,0) = 1.;
					D(1,1) = ( 1. + pow( scale , AnisotropyExponent.value ) * AnisotropyScale.value );

					// Compute the metric tensor with respect to the orig
					metric[s] = X.transpose() * D * X;
				}
			}
		);
	return metric;
}

template< unsigned int Degree >
std::vector< Point< double , Dim > >
Execute
(
	const std::vector< Point< double , Dim > > & vertices ,
	const std::vector< SimplexIndex< K > > & simplices ,
	const std::vector< Point< double , Dim > > & vectorField ,
	unsigned int softMaxP
)
{
	std::mt19937 rng( RandomSeed.value );
	std::uniform_real_distribution< double > dist( 0 , 1 );


	using NodeMultiIndex = typename SimplexMesh< Dim , Degree >::NodeMultiIndex;

	auto RandomColor = [&]( void )
		{
			Point< double , 3 > clr;
			Point< double , 3 > hsv( dist( rng )*2.*M_PI , 1. , 1. );
			Miscellany::HSVtoRGB( &hsv[0] , &clr[0] );
			return clr;
		};

	auto RandomGray = [&]( void )
		{
			return Point< double , 3 >( 1. , 1. , 1. ) * dist( rng );
		};

	Miscellany::PerformanceMeter pMeter;

	std::vector< Point< double , 3 > > colors( vertices.size() );

	// Construct the simplex mesh with the associated metric
	std::vector< SquareMatrix< double , Dim > > licMetric = LICMetric( vertices , simplices , vectorField , softMaxP );
	SimplexMesh< K , Degree > simplexMesh = SimplexMesh< K , Degree >::Init( simplices , [&]( size_t s ){ return licMetric[s]; } );
	simplexMesh.makeUnitVolume();
	if( Verbose.set ) std::cout << pMeter( "Set metric" ) << std::endl;

	Eigen::MatrixXd x( simplexMesh.nodes() , 3 );
	for( unsigned int i=0 ; i<simplexMesh.nodes() ; i++ )
	{
		Point< double , 3 > clr = GrayScale.set ? RandomGray() : RandomColor();
		for( unsigned int c=0 ; c<3 ; c++ ) x(i,c) = clr[c];
	}
	Eigen::SparseMatrix< double > M = simplexMesh.mass() , S = simplexMesh.stiffness();
	if( Verbose.set ) std::cout << pMeter( "Got system" ) << std::endl;

	// Perform the diffusion
	{
		Miscellany::PerformanceMeter pMeter;
		LLtSolver solver( M + S * DiffusionStepSize.value );
		if( Verbose.set ) std::cout << pMeter( "Factored" ) << std::endl;
		x = solver.solve( M * x );
		if( Verbose.set ) std::cout << pMeter( "Solved diffusion" ) << std::endl;
	}

	// Perform the sharpening
	if( SharpeningScale.value>0 )
	{
		Miscellany::PerformanceMeter pMeter;
		LLtSolver solver( M + S * SharpeningWeight.value );
		if( Verbose.set ) std::cout << pMeter( "Factored" ) << std::endl;
		x = solver.solve( M * x + S * x * SharpeningWeight.value * SharpeningScale.value );
		if( Verbose.set ) std::cout << pMeter( "Solved sharpening" ) << std::endl;
	}

	for( unsigned int i=0 ; i<colors.size() ; i++ )
	{
		unsigned int idx[Degree];
		for( unsigned int d=0 ; d<Degree ; d++ ) idx[d] = i;
		unsigned int I = simplexMesh.nodeIndex( NodeMultiIndex(idx) );
		for( unsigned int c=0 ; c<3 ; c++ ) colors[i][c] = x(I,c);
	}

	return colors;
}

int
main
(
	int argc ,
	char * argv[]
)
{
	using PlyVFFace = LIC::PlyVFFace< unsigned int , double , Dim >;

	CmdLineParse( argc-1 , argv+1 , params );
	if( !In.set )
	{
		ShowUsage( argv[0] );
		return EXIT_FAILURE;
	}

	srand( RandomSeed.value );

	std::stringstream sStream;

	std::vector< SimplexIndex< K > > simplices;
	std::vector< Point< double , Dim > > vertices , faceVectorField , vertexVectorField;
	std::vector< GregTurk::PlyProperty > properties;

	int file_type;
	{
		using Factory = DataFactory::Factory< double , DataFactory::PositionFactory< double , Dim > , DataFactory::VectorFieldFactory< double , Dim > >;
		using Vertex = Factory::DataType;

		Factory factory;

		std::vector< bool > vertexReadParams , faceReadParams;
		std::vector< Vertex > _vertices;
		std::vector< PlyVFFace > _faces;
		std::vector< GregTurk::PlyProperty > _properties = PlyVFFace::ReadProperties();
		file_type = PLY::ReadPolygons( In.value , factory , _vertices , _faces , &_properties[0] , static_cast< unsigned int >( _properties.size() ) , vertexReadParams , faceReadParams , nullptr );
		for( unsigned int i=0 ; i<_properties.size() ; i++ ) if( faceReadParams[i] ) properties.push_back( _properties[i] );

		simplices.resize( _faces.size() );
		for( unsigned int i=0 ; i<_faces.size() ; i++ )
		{
			if( _faces[i].size()!=(K+1) ) MK_THROW( "Expected simplex: " , _faces[i].size() , " != " , K+1 );
			for( unsigned int k=0 ; k<=K ; k++ ) simplices[i][k] = _faces[i][k];
		}

		vertices.resize( _vertices.size() );
		for( unsigned int i=0 ; i<vertices.size() ; i++ ) vertices[i] = _vertices[i].template get<0>();


		if( factory.template plyValidReadProperties< 1 >( vertexReadParams ) )
		{
			vertexVectorField.resize( _vertices.size() );
			for( unsigned int i=0 ; i<_vertices.size() ; i++ ) vertexVectorField[i] = _vertices[i].template get<1>();
		}
		else if( faceReadParams[0] && ( faceReadParams[1] || faceReadParams[4] ) && ( faceReadParams[2] || faceReadParams[5] ) && ( faceReadParams[3] || faceReadParams[6] ) )
		{
			faceVectorField.resize( simplices.size() );
			for( unsigned int i=0 ; i<simplices.size() ; i++ ) faceVectorField[i] = _faces[i].v;
		}
		else
		{
			MK_WARN( "No vector field found, setting to zero" );
			faceVectorField.resize( simplices.size() );
		}
	}

	if( vertexVectorField.size() )
	{
		for( unsigned int i=0 ; i<Subdivide.value ; i++ ) PlanarSubdivide( vertices , simplices , vertexVectorField );
		faceVectorField.resize( simplices.size() );
		for( unsigned int i=0 ; i<simplices.size() ; i++ )
		{
			Point< double , Dim > vf;
			for( unsigned int k=0 ; k<=K ; k++ ) vf += vertexVectorField[ simplices[i][k] ];
			faceVectorField[i] = vf / static_cast< double >( K+1 );
		}
	}
	else for( unsigned int i=0 ; i<Subdivide.value ; i++ ) PlanarSubdivide( vertices , simplices , faceVectorField );

	if( Verbose.set ) std::cout << "Vertices / Triangles: " << vertices.size() << " / " << simplices.size() << std::endl;

	Miscellany::Timer timer;
	std::vector< Point< double , 3 > > colors;

	{
		Miscellany::PerformanceMeter pMeter;
		switch( Degree.value )
		{
			case 1: colors = Execute< 1 >( vertices , simplices , faceVectorField , SoftMaxP.value ) ; break;
			case 2: colors = Execute< 2 >( vertices , simplices , faceVectorField , SoftMaxP.value ) ; break;
			case 3: colors = Execute< 3 >( vertices , simplices , faceVectorField , SoftMaxP.value ) ; break;
			default: MK_ERROR_OUT( "Only degrees 1, 2, and 3 supported" );
		}
		if( Verbose.set ) std::cout << pMeter( "Performed LIC" ) << std::endl;
	}

	auto ClampColor = []( Point< double , 3 > c )
	{
		for( unsigned int i=0 ; i<3 ; i++ ) c[i] = std::max< double >( 0. , std::min< double >( 1. , c[i] ) );
		return c;
	};

	if( Out.set )
	{
		if( vertexVectorField.size() )
		{
			using Factory = DataFactory::Factory< double , DataFactory::PositionFactory< double , 3 > , DataFactory::RGBColorFactory< double > , DataFactory::VectorFieldFactory< double , 3 > >;
			using Vertex = Factory::DataType;

			Factory factory;

			std::vector< Vertex > _vertices( vertices.size() );
			for( unsigned int i=0 ; i<vertices.size() ; i++ )
			{
				_vertices[i].template get<0>() = vertices[i];
				_vertices[i].template get<1>() = ClampColor( colors[i] ) * 255.;
				_vertices[i].template get<2>() = vertexVectorField[i];
			}
			PLY::WriteSimplices( Out.value , Factory() , _vertices , simplices , file_type );
		}
		else
		{
			using Factory = DataFactory::Factory< double , DataFactory::PositionFactory< double , 3 > , DataFactory::RGBColorFactory< double > >;
			using Vertex = Factory::DataType;

			std::vector< Vertex > _vertices( vertices.size() );
			for( unsigned int i=0 ; i<vertices.size() ; i++ )
			{
				_vertices[i].get<0>() = vertices[i];
				_vertices[i].get<1>() = ClampColor( colors[i] ) * 255.;
			}
			std::vector< PlyVFFace > _faces( simplices.size() );
			for( unsigned int i=0 ; i<simplices.size() ; i++ )
			{
				_faces[i].resize( K+1 );
				for( unsigned int k=0 ; k<=K ; k++ ) _faces[i][k] = simplices[i][k];
				_faces[i].v = faceVectorField[i];
			}
			PLY::WritePolygons( Out.value , Factory() , _vertices , _faces , &properties[0] , static_cast< unsigned int >( properties.size() ) , file_type , nullptr );
		}
	}

	return EXIT_SUCCESS;
}