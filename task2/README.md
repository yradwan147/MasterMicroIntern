## JSON API using C++

### Why I used C++
C++ is one of the best languages for object oriented programming as it's used as a base language to teach data structures and algorithms all the time. This is due to its
inherent speed and nature as a compiled language. C++ is also very popular as it has gained the following of a very large community who have built numerous libraries to
make coding faster and easier. C++ also, being a compiled language, can have its executables run on mostly any platform from windows, linux and mac to mobile devices and consoles.

### API Documentation
#### readJSON
Used to read a JSON topology into memory.
The syntax is as follows:
```c++
int readJSON(string filename);
```
This function will read the file into memory and return the number of topologies in memory at the moment.

#### dumpTopology
Used to write a topology from memory into a local file.
A topology list is required to be created to run this function. Given a TopologyList mem,
```c++
string dumpTopology(string TopologyID);
// Calling the function
mem.dumpTopology("top1");
```
This function will return "Output is written" if write is successful. The output filename is the id of the topology.

#### getTopologies
Used to get all topologies in memory.
A topology list is required to be created to run this function. Given a TopologyList mem,
```c++
string getTopologies();
// Calling the function
mem.getTopologies();
```
This function will return a string including all topologies in memory and their id.

Result Example:
```
Topology top1
```

#### getTopologyDevices
Used to get all devices inside a topology in memory.
A topology list is required to be created to run this function. Given a TopologyList mem,
```c++
string getTopologyDevices(string TopologyID);
// Calling the function
mem.getTopologyDevices("top1");
```
This function returns all devices in a certain topology in memory accompanied with all their details.

Result Example:
```
ID:res1
Type:resistor
Properties(resistance):
default:100.000000
max:1000.000000
min:10.000000

NetList:
t1:vdd
t2:n1

--------
ID:m1
Type:nmos
Properties(m(l)):
default:1.500000
max:2.000000
min:1.000000

NetList:
drain:n1
gate:vin
source:vss

--------
```

#### deleteTopology
Used to delete a topology from memory. 
A topology list is required to be created to run this function. Given a TopologyList mem,
```c++
string deleteTopology(string TopologyID)
// Calling the function
mem.deleteTopology("top1");
```
This function returns the ID of the deleted topology

#### getTopologyDevicesNetList
Used to get devices in a certain topology in memory that include a certain netlist device.
A topology list is required to be created to run this function. Given a TopologyList mem,
```c++
string getTopologyDevicesNetList(string TopologyID, string NetID);
// Calling the function
mem.getTopologyDevicesNetList("top1", "vss");
```
This function returns all devices in that topology that include the netlist device

Result Example:
```
ID:m1
Type:nmos
Properties(m(l)):
default:1.500000
max:2.000000
min:1.000000

NetList:
drain:n1
gate:vin
source:vss

--------
```


