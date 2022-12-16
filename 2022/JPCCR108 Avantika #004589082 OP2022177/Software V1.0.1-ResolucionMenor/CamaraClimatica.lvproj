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
				<Property Name="App_INI_aliasGUID" Type="Str">{337D26F3-EE6F-4433-97AE-1CD60F2C36D6}</Property>
				<Property Name="App_INI_GUID" Type="Str">{14766766-FD40-4409-A80E-000CC4DE6523}</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C44F48E9-BD1F-49A3-B626-A950FF9FDF2A}</Property>
				<Property Name="Bld_buildSpecDescription" Type="Str">1,0,2</Property>
				<Property Name="Bld_buildSpecName" Type="Str">CamaraClimatica_Viewer</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/NI_AB_PROJECTNAME/CamaraClimatica_Viewer</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{9CCC41E3-F3C6-4E09-8930-C385CC64D086}</Property>
				<Property Name="Destination[0].destName" Type="Str">CamaraClimatica_Viewer.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/NI_AB_PROJECTNAME/CamaraClimatica_Viewer/CamaraClimatica_Viewer.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/NI_AB_PROJECTNAME/CamaraClimatica_Viewer/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{C4D1C62B-DE58-4578-9B3A-9B0BC35D8C21}</Property>
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
				<Property Name="TgtF_targetfileGUID" Type="Str">{AA9926DF-5357-4BBF-B2F1-CC69A9E7A813}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">CamaraClimatica_Viewer.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
