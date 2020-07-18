//#pragma once
//#ifndef _MapPointField_Header
//#define _MapPointField_Header
//
///*---------------------------------------------------------------------------*\
//  =========                 |
//  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
//   \\    /   O peration     | Website:  https://openfoam.org
//	\\  /    A nd           | Copyright (C) 2011-2019 OpenFOAM Foundation
//	 \\/     M anipulation  |
//-------------------------------------------------------------------------------
//License
//	This file is part of OpenFOAM.
//
//	OpenFOAM is free software: you can redistribute it and/or modify it
//	under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
//	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//	for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.
//
//Class
//	tnbLib::MapPointField
//
//Description
//	Map point field on topology change. This is a partial template
//	specialisation for GeoMesh=pointMesh
//
//See also
//	MapGeometricFields
//
//\*---------------------------------------------------------------------------*/
//
//#include <Field.hxx>
//#include <pointMesh.hxx>
//
//// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//
//namespace tnbLib
//{
//
//	template<class Type, class MeshMapper>
//	class MapInternalField<Type, MeshMapper, pointMesh>
//	{
//	public:
//
//		MapInternalField()S
//		{}
//
//		void operator()
//			(
//				Field<Type>& field,
//				const MeshMapper& mapper
//				) const;
//	};
//
//
//	template<class Type, class MeshMapper>
//	void MapInternalField<Type, MeshMapper, pointMesh>::operator()
//		(
//			Field<Type>& field,
//			const MeshMapper& mapper
//			) const
//	{
//		if (field.size() != mapper.pointMap().sizeBeforeMapping())
//		{
//			FatalErrorInFunction
//				<< "Incompatible size before mapping.  Field size: " << field.size()
//				<< " map size: " << mapper.pointMap().sizeBeforeMapping()
//				<< abort(FatalError);
//		}
//
//		mapper.pointMap()(field, field);
//	}
//
//
//	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//
//} // End namespace tnbLib
//
//// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//
//#endif // !_MapPointField_Header