#include <iostream>
#include <nlohmann/json.hpp>

#include <map>
#include <vector>
#include <fstream>
#include <string>


using namespace std;

// Abbreviating import
using json = nlohmann::json;


class Device{

private:
    string id;
    map<string,double> properties;
    string properties_name;
    map<string,string> netlist;
    string type;
public:
    Device(string id_in, map<string,double> prop_in, map<string,string> net_in, string type_in, string prop_name){
        id = id_in;
        properties = prop_in;
        netlist = net_in;
        type = type_in;
        properties_name = prop_name;
    }
    
    // Return formatted string with all device details
    string deviceDetails(){

        string properties_str = "";
        string netlist_str = "";
        // Loop through map and stringify key:value pairs
        for (auto const& keyValue : properties)
        {
          properties_str = properties_str + keyValue.first + ":" + to_string(keyValue.second) +"\n";
        }
        for (auto const& keyValue : netlist)
        {
          netlist_str = netlist_str + keyValue.first + ":" + keyValue.second +"\n";
        }
        string res = "ID:" + id + "\nType:" +  type + "\nProperties("+properties_name+"):\n" +  properties_str + "\nNetList:\n" +  netlist_str + "\n" +  "--------"+"\n";
        return res;
    }
    
    // Put device back into json form as a string and return it to be concatenated
    string JSONDump(){
        string res = "{\"type\":\""+type+"\",\"id\":\""+id+"\",\""+properties_name+"\":{";
        for (auto const& keyValue : properties)
        {
          res = res + "\"" + keyValue.first + "\":" + to_string(keyValue.second) +",";
        }
        res.pop_back();
        res = res + "},\"netlist\":{";
        for (auto const& keyValue : netlist)
        {
          res = res + "\"" + keyValue.first + "\":\"" + keyValue.second +"\",";
        }
        res.pop_back();
        res = res+"}},";
        return res;
    }
    // Check if device in netlist
    bool inNetList(string dev){
        for (auto& iter: netlist){
            if (iter.second == dev){
                return true;
            }
        };
        return false;
    }
    


};


class Topology{
private:
    string id;
    vector<Device> devList;

public:
    Topology(){
        id = "";
    }
    Topology(string id_in){
        id = id_in;
    }
    // ID getter
    string get_id(){
        return id;
    }

    // Add device
    void AddDevice(Device dev){
        devList.push_back(dev);
    }

    // Return string result with concatenated device details
    string GetDevices(){
        string res = "";
        for (auto& x: devList){
            res = res + x.deviceDetails();
        }
        if (res == ""){
            res = "No Devices Found!";
        }
        return res;
    }

    // Loop through deviceList vector and call inNetList to find devices in topology with netlist device
    string GetDevicesNetList(string netID){
        string res = "";
        for (auto& x: devList){
            if (x.inNetList(netID)){
                res = res + x.deviceDetails();
            }
        }
        if (res == ""){
            res = "No Devices Found!";
        }
        return res;
    }

    // Run JSONDump for all devices in deviceList and concatenate into json form as a string and convert to json
    json TopJSONDump(){
        string res = "{\"id\":\"" + id +"\",\"components\":[";
        for (auto& x: devList){
            res = res + x.JSONDump();
        }
        res.pop_back();
        res = res+"]}";
        json j = json::parse(res);
        return j;
    }


};

class TopologyList{


private:
    // This vector represents local memory
    vector<Topology> memory;
public:
    // Add topology to local memory
    int addTopology(Topology top){
        memory.push_back(top);
        return memory.size();
    }

    // Return string with details for all topologies in memory
    string getTopologies(){
        string res = "";
        for (auto& topology: memory){
            res = res + "Topology " + topology.get_id() + "\n";
        }
        if (res == ""){
            res = "No Topologies Found!";
        }
        return res;
    }
    // Return devices in specific topology in memory
    string getTopologyDevices(string TopologyID){
        for (auto& topology: memory){
            if (topology.get_id() == TopologyID){
                return topology.GetDevices();
            }
        }
        return "No Topology with given ID";
    }

    // Return devices with netlist device in a specific topology in memory
    string getTopologyDevicesNetList(string TopologyID, string NetID){
        for (auto& topology: memory){
            if (topology.get_id() == TopologyID){
                return topology.GetDevicesNetList(NetID);
            }
        }
        return "No Topology with given ID";
    }

    // Delete topology from memory
    string deleteTopology(string TopologyID){
        int  i = -1;
        Topology temp;
        for (auto& topology: memory){
            i = i + 1;
            if (topology.get_id() == TopologyID){
                temp = topology;
                break;
            }
        }
        memory.erase(memory.begin()+i);
        return temp.get_id();
    }

    // Top-level JSONDump and output json into file with id as filename
    string dumpTopology(string TopologyID){
        for (auto& topology: memory){
            if (topology.get_id() == TopologyID){
                json j = topology.TopJSONDump();
                ofstream o(topology.get_id()+".json");
                o << setw(4) << j << endl;
                return "Output Written\n";
            }
        }
        return "No Topology with given ID";
    }


};

TopologyList mem;


// Read JSON into memory
int readJSON(string filename){

    // Read JSON file into variable
    ifstream i(filename);
    json j;
    i >> j;

    // Create toplogy with id constructor
    Topology top(j["id"]);
    // Loop through components to create Device object for each device in components
    for(auto& iter: j["components"]) {
        string properties_key;
        // Loop through properties to find key for device properties map
        for (auto& el : iter.items()) {
            if(el.key() != "id" && el.key() != "type" && el.key() != "netlist"){
                properties_key = el.key();
            }
        };
        // Create Device and add to topology deviceList
        Device d(iter["id"], iter[properties_key], iter["netlist"], iter["type"], properties_key);
        top.AddDevice(d);
    }
    // Return number of JSONs in memory
    int count = mem.addTopology(top);
    return count;

}



int main ()
{   

    cout << readJSON("topology.json") << endl;
    cout << readJSON("topology.json") << endl;
    cout << mem.dumpTopology("top1")<<endl;
    cout << mem.getTopologies() << endl;
    cout << mem.getTopologyDevices("top1") << endl;
    cout << mem.deleteTopology("top1") << endl;
    cout << mem.getTopologyDevicesNetList("top1", "vss") << endl;


}