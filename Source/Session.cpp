//
//  Session.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/18/21.
//

#include "Session.hpp"

Session::Session(ClientList* cl, InfoPanel* ip) : mClientList(cl), mInfoPanel(ip)
{
	//	acquire resource directory for loading and saving
	auto locationType = juce::File::SpecialLocationType::currentApplicationFile;
	auto dir = juce::File::getSpecialLocation(locationType);
	int numTries = 0;
	//	backtracking until it finds resources
	while (! dir.getChildFile ("Resources").exists() && numTries++ < 15)
		dir = dir.getParentDirectory();
	mResourceDir = dir.getChildFile("Resources");
	//	load the table headers
	loadTableHeaders(mResourceDir.getChildFile("TableHeaders.xml"));
	
//	createClient("test", 0, 1, true, true, true);
}

Session::~Session()
{
    freeClients();
}
void Session::saveSession()
{
	DBG("Saving Session...");
}

void Session::loadSession(juce::File sessionFile)
{
	DBG("Loading Session...");
	
	if (sessionFile.exists())
	{
//		std::unique_ptr<juce::XmlElement> clientData;
//		clientData = juce::XmlDocument::parse(sessionFile);
//		
//		juce::XmlElement* columnList = clientData->getChildByName ("DATA");
//		juce::XmlElement* dataList = clientData->getChildByName ("HEADERS");
//		
//		int numRows = dataList->getNumChildElements();
//		
//		forEachXmlChildElement(*columnList, columnXml)
//		{
//			
//			DBG(columnXml->getStringAttribute ("name"));
//			DBG(columnXml->getStringAttribute ("columnId"));
//			DBG(columnXml->getStringAttribute ("width"));
//			DBG("\n");
//
//
//			mTable.getHeader().addColumn (columnXml->getStringAttribute ("name"),
//										 columnXml->getIntAttribute ("columnId"),
//										 columnXml->getIntAttribute ("width"),
//										 50,
//										 400,
//										 juce::TableHeaderComponent::defaultFlags);
//		}
	}
	
//	set mClientList.mNumRows = dataList->getNumChildElements()
}

void Session::loadTableHeaders(juce::File xmlTableHeaders)
{
	std::unique_ptr<juce::XmlElement> tableHeaderPtrs = juce::XmlDocument::parse(xmlTableHeaders);
	
	juce::XmlElement* columnList = tableHeaderPtrs->getChildByName ("HEADERS");
	
	
//	foreach tableheader -> add to column on clientList
	forEachXmlChildElement(*columnList, colXml)
	{
		auto name = colXml->getStringAttribute("name");
		juce::String columnId = colXml->getStringAttribute("columnId");
		juce::String width = colXml->getStringAttribute("width");
		
		mClientList->addHeaderColumn(name,
									 columnId.getIntValue(),
									 width.getIntValue());
	}
}

void Session::update()
{
	int selectedClient = mClientList->getLatestSelection();
//	then take this selected client, and send update to infoPanel
	
	
//	mInfoPanel
}

void Session::createClient(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage)
{
	if(nameExists(name))
		name = findAlternateName(name);
	
	if (port == -1) // -1 specified in interface to be no input
		port = findEmptyPort();
	

	Client* newClient = new Client(name, port, channels, autoConnectAudio, zeroUnderrun, autoManage);
	mAllClients.add(newClient);
}

void Session::freeClients()
{
    for (Client* client : mAllClients)
        delete client;
}

// finds the lowest value empty port
int Session::findEmptyPort()
{
	int emptyPort = PORT_RNG_START;
	
	juce::Array<int> takenPorts;
	
	for (Client* client : mAllClients)
		takenPorts.add(client -> getPort());
	
	
	while (takenPorts.contains(emptyPort))
		emptyPort++;
	
	return emptyPort;
}

bool Session::nameExists(juce::String name)
{
	for (Client* client : mAllClients)
		if(client -> compareName(name))
			return true;
	return false;
}

juce::String Session::findAlternateName(juce::String name)
{
	juce::String altName = name;
	int i = 0;
	while (nameExists(name))
	{
		altName = name + juce::String(" (" + std::to_string(i) + ")");
		i++;
	}
	return altName;
}
