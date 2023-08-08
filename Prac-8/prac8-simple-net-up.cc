#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("FirstScriptExample");
int main(int argc, char *argv[])
{
CommandLine cmd(__FILE__);
cmd.Parse(argc, argv);
LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
NodeContainer nodes;
NodeContainer routers;
nodes.Create(1);
routers.Create(4);
NodeContainer csmaNodes;
csmaNodes.Create(3);
InternetStackHelper stack;
stack.Install(nodes);
stack.Install(routers);
stack.Install(csmaNodes);
// subnet1
PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
NodeContainer subnet1;
subnet1.Add(nodes.Get(0));
subnet1.Add(routers.Get(0));
NetDeviceContainer Subnet1devices;
Subnet1devices = pointToPoint.Install(subnet1);
Ipv4AddressHelper subnet1address;
subnet1address.SetBase("10.1.1.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces1;
p2pInterfaces1 = subnet1address.Assign(Subnet1devices);
// subnet2
NodeContainer subnet2;
subnet2.Add(routers.Get(0));
subnet2.Add(routers.Get(1));
NetDeviceContainer Subnet2devices;
Subnet2devices = pointToPoint.Install(subnet2);
Ipv4AddressHelper subnet2address;
subnet2address.SetBase("10.1.2.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces2;
p2pInterfaces2 = subnet2address.Assign(Subnet2devices);
// subnet3
NodeContainer subnet3;
subnet3.Add(routers.Get(1));
subnet3.Add(routers.Get(2));
NetDeviceContainer Subnet3devices;
Subnet3devices = pointToPoint.Install(subnet3);
Ipv4AddressHelper subnet3address;
subnet3address.SetBase("10.1.3.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces3;
p2pInterfaces3 = subnet3address.Assign(Subnet3devices);
// subnet4
NodeContainer subnet4;
subnet4.Add(routers.Get(1));
subnet4.Add(routers.Get(3));
NetDeviceContainer Subnet4devices;
Subnet4devices = pointToPoint.Install(subnet4);
Ipv4AddressHelper subnet4address;
subnet4address.SetBase("10.1.4.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces4;
p2pInterfaces4 = subnet4address.Assign(Subnet4devices);
// subnet5
CsmaHelper csma;
csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));
NodeContainer subnet5;
subnet5.Add(csmaNodes.Get(0));
subnet5.Add(csmaNodes.Get(1));
subnet5.Add(csmaNodes.Get(2));
subnet5.Add(routers.Get(2));
NetDeviceContainer csmaDevices;
csmaDevices = csma.Install(subnet5);
Ipv4AddressHelper subnet5address;
subnet5address.SetBase("10.1.5.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces5;
p2pInterfaces5 = subnet5address.Assign(csmaDevices);
UdpEchoServerHelper echoServer(9);
ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(0));
serverApps.Start(Seconds(1.0));
serverApps.Stop(Seconds(10.0));
UdpEchoClientHelper echoClient(p2pInterfaces5.GetAddress(0), 9);
echoClient.SetAttribute("MaxPackets", UintegerValue(1));
echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
echoClient.SetAttribute("PacketSize", UintegerValue(1024));
ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
clientApps.Start(Seconds(2.0));
clientApps.Stop(Seconds(10.0));
Ipv4GlobalRoutingHelper::PopulateRoutingTables();
pointToPoint.EnablePcap("p2pSubnet", Subnet1devices.Get(0), true);
csma.EnablePcap("csmaSubnet", csmaDevices.Get(0), true);
AnimationInterface anim("Subnet.xml");
Simulator::Run();
Simulator::Destroy();
return 0;
}
