//
//  Session.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/18/21.
//

#include "Session.hpp"

Session::Session(ClientList* cl, InfoPanel* ip) : mClientList(cl), mInfoPanel(ip), sess_AllClientInfo("clientInfoXml-Session")
{
	mClientList->setInitPtrs(&sess_AllClients, &sess_AllClientInfo);
	
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
//	UNCOMMENT loadSession TO LOAD WHOLE SESSION, just like this now to avoid heavier load on debugging
//	loadSession(mResourceDir.getChildFile("SessionData.xml"));
	
	createClient("test1", 0, 1, true, true, true);
	createClient("test2", 1, 1, true, true, true);
	createClient("test3", 2, 1, true, true, true);

//	mClientList->setNumRows(sess_AllClients.size());
	
	DBG("LOADED SESSION");
}

Session::~Session()
{
    freeClients();
	
//	delete sess_AllClientInfo;
}

void Session::saveSession()
{
	DBG("Saving Session...");
	
//	XmlElement sessionXml("savedSessionXml");
//	for (Client* client : sess_AllClients)
//		sessionXml.addChildElement(client->getClientInfo());
	
	auto xmlString = sess_AllClientInfo.toString();
		
	FileChooser fc("Save Session", mResourceDir);
	
	if (fc.browseForFileToSave(true))
	{
		File saveFile = fc.getResult();
		saveFile.withFileExtension("xml").replaceWithText(xmlString);
	}
}

void Session::loadSession()
{
	DBG("Loading Session...");
	
	FileChooser fc("Load Session", mResourceDir);
	
	File sessionFileToOpen;
	
	if (fc.browseForFileToOpen())
	{
		sessionFileToOpen = fc.getResult();
	}
	
	if (sessionFileToOpen.exists())
	{
		std::unique_ptr<juce::XmlElement> clientData;
		clientData = juce::XmlDocument::parse(sessionFileToOpen);
		
		juce::XmlElement* dataList = clientData->getChildByName ("DATA");
		
		// we no longer set num rows here, instead each new client does that
		// mClientList->setNumRows(dataList->getNumChildElements());
		
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
	
	// requests that clients record their xml info to the shared ptr sess_AllClientInfo
	broadcastClientUpdate();
	
	int selectedClient = mClientList->getLatestSelection();
	
	// get the stats of currently selected client
	auto selectedClientInfo = sess_AllClientInfo.getChildElement(selectedClient);
	
	// then take this selected client, and send update to infoPanel
	//	mInfoPanel.updateDisplay()
	mInfoPanel->updateDisplay(selectedClientInfo);
	
	
//	DBG(sess_AllClient[0]->getOutput());
	
	if (debugSessionUpdateTime)
		DBG("update finished after " + String(Time::getHighResolutionTicks() - t1));
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
	
	mClientList->setNumRows(sess_AllClients.size());
	
	DBG("NEW CLIENT " + name + " CREATED");
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
		altName = name + " " + juce::String(i);
		i++;
	}
	return altName;
}

// ==== LOADING / SAVING =====================================

void Session::loadTableHeaders(juce::File xmlTableHeaders)
{
	//	FIRST, WE NEED TO DELETE ALL OF THE EXISTING COLUMN HEADERS
	mTableHeadPtr = juce::XmlDocument::parse(xmlTableHeaders);
	
	juce::XmlElement* columnList = mTableHeadPtr->getChildByName ("HEADERS");
	
	mClientList->setColumnHeaders(columnList);
}

XmlElement Session::getClientXmlStats()
{
	XmlElement allClientsXml("allClientsXml");
	
	for(Client* client : sess_AllClients)
		allClientsXml.addChildElement(client->getClientInfo());
	return allClientsXml;
}

void Session::broadcastClientUpdate()
{
	for(Client* client : sess_AllClients)
		client->recordClientInfo(); // will update xmltree for clients part
}
