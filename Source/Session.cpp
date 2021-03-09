//
//  Session.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/18/21.
//

#include "Session.hpp"

Session::Session(ClientList* cl, InfoPanel* ip) : mClientList(cl), mInfoPanel(ip), sess_AllClientInfo("clientInfoXml-Session")
{
//	sess_AllClientInfo = new XmlElement("clientInfoXml-Session");
	
	mClientList->setInitPtrs(sess_AllClients, &sess_AllClientInfo);
	
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

//	Make method to auto load last saved session
//	loadSession(mResourceDir.getChildFile("SessionData.xml"));
	
	createClient("test1", 0, 1, true, true, true);
	createClient("test2", 1, 1, true, true, true);
	createClient("test2", 2, 1, true, true, true);

	mClientList->setNumRows(sess_AllClients.size());
	

	DBG("LOADED SESSION");
}

Session::~Session()
{
    freeClients();
	
//	delete sess_AllClientInfo;
}

void Session::saveSession(File sessionFileToSave)
{
	DBG("Saving Session...");
	
	XmlElement sessionXml("savedSessionXml");
	
	for (Client* client : sess_AllClients)
		sessionXml.addChildElement(client->getClientInfo());
	
	auto xmlString = sessionXml.toString();
	
	// TODO - add way to save xmlString string
}

void Session::loadSession(juce::File sessionFileToOpen)
{
	DBG("Loading Session...");
	
	if (sessionFileToOpen.exists())
	{
		std::unique_ptr<juce::XmlElement> clientData;
		clientData = juce::XmlDocument::parse(sessionFileToOpen);
		
		juce::XmlElement* dataList = clientData->getChildByName ("DATA");
		
		// 	set number of rows in clientList
		mClientList->setNumRows(dataList->getNumChildElements());
		
		//	before we load session, we need to delete all existing column headers and replace them with whatever is in the header of the session file. This will only affect the horizontal sizing of cells from last session
		mClientList->resetColumns();
		loadTableHeaders(sessionFileToOpen);
		
		// 	delete existing clients; create a popup here that confirms if user wants to delete current session
		freeClients();
		
		// create the clients as objects
		forEachXmlChildElement(*dataList, dataXml)
		{
			// create a new client with the xml row information
			createClient(dataXml->getStringAttribute("Name"),
						 dataXml->getStringAttribute("Port").getIntValue(),
						 dataXml->getStringAttribute("Channels").getIntValue(),
						 dataXml->getStringAttribute("RouteAudio").getIntValue(),
						 dataXml->getStringAttribute("ZeroUnderrun").getIntValue(),
						 dataXml->getStringAttribute("AutoManage").getIntValue());
		}
	}
}



void Session::update()
{
	auto t1 = Time::getHighResolutionTicks();
	
	int selectedClient = mClientList->getLatestSelection();
	// then take this selected client, and send update to infoPanel
//	mInfoPanel.updateDisplay()
	
	auto test = sess_AllClientInfo.getChildElement(0);
	
//	DBG(String(test->getStringAttribute("Name")));
	// update clientLists's xml of client stats. NOTE: I really hate it this way, what other ways can we provide proper column and row based information to ClientList::paintCell, which is an override called by other juce components? Edit the source? This just seems really inefficient to be constantly generating xml
	
	// dont know if juce calls paintCell before or after this (probably before), meaning this is all around dumb
	
	auto updateTime = Time::getHighResolutionTicks() - t1;
	DBG("update finished after " + String(updateTime));

}

void Session::createClient(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage)
{
	if(nameExists(name))
		name = findAlternateName(name);
	
	// add a checkIfPortUsed method, which may require a popup alterting user that port is already in use, in which case they can find next available or boot other client off port
	if (port == -1) // -1 is default, will automatically find port
		port = findEmptyPort();
	
	Client* newClient = new Client(name, port, channels, autoConnectAudio, zeroUnderrun, autoManage, true);
	
//	now get the pointer to its xmlElement, add it as a child to sessAllClientInfo
	sess_AllClientInfo.addChildElement(newClient->getClientInfo());
	
	sess_AllClients.add(newClient);
}

void Session::freeClients()
{
    for (Client* client : sess_AllClients)
        delete client;
}

// finds the lowest value empty port
int Session::findEmptyPort()
{
	int emptyPort = PORT_RNG_START;
	
	juce::Array<int> takenPorts;
	
	for (Client* client : sess_AllClients)
		takenPorts.add(client -> getPort());
	
	
	while (takenPorts.contains(emptyPort))
		emptyPort++;
	
	return emptyPort;
}

bool Session::nameExists(juce::String name)
{
	for (Client* client : sess_AllClients)
		if(client -> compareName(name))
			return true;
	return false;
}

juce::String Session::findAlternateName(juce::String name)
{
	juce::String altName = name;
	int i = 0;
	while (nameExists(altName))
	{
		altName = name + " (" + juce::String(i) + ")";
		i++;
	}
	return altName;
}

// ==== LOADING / SAVING =====================================

void Session::loadTableHeaders(juce::File xmlTableHeaders)
{
//	FIRST, WE NEED TO DELETE ALL OF THE COLUMN HEADERS
//	std::unique_ptr<juce::XmlElement> tableHeaderPtrs = juce::XmlDocument::parse(xmlTableHeaders);
	mTableHeadPtr = juce::XmlDocument::parse(xmlTableHeaders);
	
	juce::XmlElement* columnList = mTableHeadPtr->getChildByName ("HEADERS");
	
	mClientList->setColumnHeaders(columnList);
//	forEachXmlChildElement(*columnList, colXml)
//	{
//		auto name = colXml->getStringAttribute("name");
//		juce::String columnId = colXml->getStringAttribute("columnId");
//		juce::String width = colXml->getStringAttribute("width");
//
//		mClientList->addHeaderColumn(columnList,
//									 name,
//									 columnId.getIntValue(),
//									 width.getIntValue(),
//									 25); // min width
//	}
}

XmlElement Session::getClientXmlStats()
{
	XmlElement allClientsXml("allClientsXml");
	
	for(Client* client : sess_AllClients)
		allClientsXml.addChildElement(client->getClientInfo());
	DBG(allClientsXml.getAllSubText());
	return allClientsXml;
}

void Session::broadcastClientUpdate()
{
	for(Client* client : sess_AllClients)
		client->recordClientInfo(); // will update xmltree for clients part
}
