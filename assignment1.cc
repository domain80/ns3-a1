#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/olsr-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"
#include "ns3/wifi-helper.h"
#include "ns3/yans-wifi-helper.h"
#include <iostream>
#include <fstream>
#include <string>
//#include "ns3/basic-energy-source.h"
//#include "ns3/simple-device-energy-model.h"



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int main (int argc, char *argv[])
{
  
  // Obtain command line arguments
  
  uint32_t  cols = 10;
  uint32_t  rows = 10;
  uint32_t  numNodes = 20;
  uint32_t  nodeSpacing = 3;
  uint32_t  duration = 20;
  float seed = 1.0;
  uint32_t  run = 5;
  uint32_t  packetRate = 10;
  uint32_t  packetSize = 1024;
  uint32_t  sourceNode = 1;
  uint32_t  destinationNode = 18;
  bool showSimTime = true;

  CommandLine cmd;
  cmd.AddValue ("cols", "Columns of nodes", cols);
  cmd.AddValue ("rows", "Columns of nodes", rows);
  cmd.AddValue ("numNodes", "Number of nodes", numNodes);
  cmd.AddValue ("nodeSpacing", "Spacing between neighbouring nodes", nodeSpacing);
  cmd.AddValue ("duration", "Duration of simulation", duration);
  cmd.AddValue ("seed", "Random seed for simulation", seed);
  cmd.AddValue ("run", "Simulation run", run);
  cmd.AddValue ("packetRate", "Packets transmitted per second", packetRate);
  cmd.AddValue ("packetSize", "Packet size", packetSize);
  cmd.AddValue ("sourceNode", "Number of source node", sourceNode);
  cmd.AddValue ("destinationNode", "Number of destination node", destinationNode);
  cmd.AddValue ("showSimTime", "show ... time ... (default = true)", showSimTime);
  //cmd.AddValue ("seed", "Randomize simulation", seed);
  cmd.Parse (argc,argv);

  // Set default parameter values
  Config::SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold",
  StringValue ("2200"));
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold",
  StringValue ("2200"));
  // Set random seed and run number
  SeedManager::SetSeed (seed);
  SeedManager::SetRun (run);
   
  // Create notes
  NodeContainer c;
  c.Create (numNodes);
 // Ptr<Application> app = node­>GetApplication(0);
  //Ptr<NetDevice> nic = node­>GetDevice(0);
  //apps = client.Install (c.Get (sourceNode));

  // Set up physical and mac layers
//= WifiHelper::Default ();

  WifiHelper wifi ;
  wifi.SetStandard (ns3::WIFI_STANDARD_80211ac);
  wifi.SetRemoteStationManager ("ns3::MinstrelHtWifiManager");

  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac");

  YansWifiPhyHelper phy = YansWifiPhyHelper();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  phy.SetChannel (wifiChannel.Create ());
  NetDeviceContainer devices = wifi.Install (phy, wifiMac, c);

  // Routing and Internet stack
  ns3::OlsrHelper olsr;
  InternetStackHelper internet;   
  internet.SetRoutingHelper(olsr);
  internet.Install (c);
  
  // Assign addresses 
  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);
  
  // Server/Receiver
  UdpServerHelper server(40);
  ApplicationContainer serverapps = server.Install(c.Get(destinationNode));
  serverapps.Start (Seconds (0.0));
  serverapps.Stop (Seconds (duration-1));
  
  /*UdpServerHelper myServer (9);
              serverApp = myServer.Install (wifiStaNode.Get (0));
              serverApp.Start (Seconds (0.0));
              serverApp.Stop (Seconds (simulationTime + 1));
  */
  // Client/Sender
  UdpClientHelper client (interfaces.GetAddress (destinationNode), 40);
  client.SetAttribute ("MaxPackets", UintegerValue (100));
  client.SetAttribute ("Interval", TimeValue (Seconds(1 / ((double) packetRate))));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  
  ApplicationContainer cleintapss;
  cleintapss = client.Install (c.Get (sourceNode));
  cleintapss.Start (Seconds (0.0));
  cleintapss.Stop (Seconds (duration-1));

  // Set up initial node locations
  MobilityHelper mobility;
  mobility.SetPositionAllocator (
  	"ns3::GridPositionAllocator",
  	"MinX", DoubleValue (1.0),
  	"MinY", DoubleValue (1.0),
  	"DeltaX", DoubleValue (nodeSpacing),
  	"DeltaY", DoubleValue (nodeSpacing),
  	"GridWidth", UintegerValue (cols));

  // Setup Mobility Model
  mobility.SetMobilityModel (
  	"ns3::RandomWalk2dMobilityModel",
  	"Bounds", RectangleValue(Rectangle (0,(cols * nodeSpacing) + 1, 0,(rows * nodeSpacing) + 1)),
  	"Speed", StringValue("ns3::UniformRandomVariable[Min=5.0|Max=10.0]"),
 	"Distance", DoubleValue(30));
	 mobility.Install (c);

  
  // Print simulated time
  //if(showSimTime)
	//Simulator::Schedule(Seconds(1), &PrintSeconds);
  
  //AnimationInterface anim ("resutls/assignment1.xml"); // Mandatory
 // std::string strNumNodes = std::to_string(numNodes);
  //std::string strSeed = std::to_string(seed);
  //std::string outputfile = "results/output-" + strNumNodes + "-" + strSeed + ".tr";
  
  AsciiTraceHelper ascii;
  
  Ptr<OutputStreamWrapper> outStream = ascii.CreateFileStream("results/output.tr", std::ios::out);
  phy.EnableAsciiAll (outStream);
  
  //phy.EnablePcap ("results/assignment", devices.Get(19), false);


  Simulator::Stop(Seconds(duration));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;

}

void PrintSeconds(void) {
	std::cerr << Simulator::Now() << std::endl;
	Simulator::Schedule(Seconds(1), &PrintSeconds);
}
