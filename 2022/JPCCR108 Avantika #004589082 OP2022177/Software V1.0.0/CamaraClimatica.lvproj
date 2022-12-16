<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="11008008">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="CamaraClimatica.vi" Type="VI" URL="../CamaraClimatica.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="BulkUSB_MN (subvi).vi" Type="VI" URL="../BulkUSB_MN (subvi).vi"/>
			<Item Name="mpusbapi.dll" Type="Document" URL="/C/pic32_solutions/USB Tools/MCHPUSB Custom Driver/Mpusbapi/Dll/Borland_C/mpusbapi.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="CamaraClimatica_Viewer" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{56516255-2B99-4C13-AC7B-40EA51D19EDE}</Property>
				<Property Name="App_INI_GUID" Type="Str">{80C3B7DE-B0B3-4E38-A878-70CF4ED540EE}</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{20F4DA3F-FC1C-4587-97A1-308F5BBBA9B3}</Property>
				<Property Name="Bld_buildSpecDescription" Type="Str">1,0,1</Property>
				<Property Name="Bld_buildSpecName" Type="Str">CamaraClimatica_Viewer</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/NI_AB_PROJECTNAME/CamaraClimatica_Viewer</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{D562F11F-09DB-4709-A31B-16F7B0E0DBED}</Property>
				<Property Name="Destination[0].destName" Type="Str">CamaraClimatica_Viewer.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/NI_AB_PROJECTNAME/CamaraClimatica_Viewer/CamaraClimatica_Viewer.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/NI_AB_PROJECTNAME/CamaraClimatica_Viewer/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{3B55C1C9-D159-4381-A69D-7D158C567705}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/CamaraClimatica.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">CamaraClimatica_Viewer</Property>
				<Property Name="TgtF_fileVersion.major" Type="Int">1</Property>
				<Property Name="TgtF_internalName" Type="Str">CamaraClimatica_Viewer</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2022 </Property>
				<Property Name="TgtF_productName" Type="Str">CamaraClimatica_Viewer</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{5EAB870E-1952-49C1-B5C6-A8C9DAC1AEE3}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">CamaraClimatica_Viewer.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
