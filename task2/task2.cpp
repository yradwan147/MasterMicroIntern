#include <iostream>
#include <nlohmann/json.hpp>

#include <map>
#include <vector>
#include <fstream>
#include <string>


using namespace std;

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

    string deviceDetails(){

        string properties_str = "";
        string netlist_str = "";
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

    bool inNetList(string dev){
        // if ( netlist.find(dev) == netlist.end() ) {
        //   return false;
        // } else {
        //   return true;
        // }
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

    string get_id(){
        return id;
    }

    void AddDevice(Device dev){
        devList.push_back(dev);
    }

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

    json TopJSONDump(){
        string res = "{\"id\":\"" + id +"\",\"components\":[";
        for (auto& x: devList){
            res = res + x.JSONDump();
        }
        res.pop_back();
        res = res+"]}";
        // doesnt work
        // cout << res << endl;
        json j = json::parse(res);
        return j;
    }


};

class TopologyList{


private:
    vector<Topology> memory;
public:
    int addTopology(Topology top){
        memory.push_back(top);
        return memory.size();
    }
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

    string getTopologyDevices(string TopologyID){
        for (auto& topology: memory){
            if (topology.get_id() == TopologyID){
                return topology.GetDevices();
            }
        }
        return "No Topology with given ID";
    }

    string getTopologyDevicesNetList(string TopologyID, string NetID){
        for (auto& topology: memory){
            if (topology.get_id() == TopologyID){
                return topology.GetDevicesNetList(NetID);
            }
        }
        return "No Topology with given ID";
    }

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

    string dumpTopology(string TopologyID){
        for (auto& topology: memory){
            if (topology.get_id() == TopologyID){
                json j = topology.TopJSONDump();
                //TODO fix
                ofstream o(to_string(topology.get_id())+".json");
                o << setw(4) << j << endl;
                return "Output Written\n";
            }
        }
        return "No Topology with given ID";
    }


};

TopologyList mem;


int readJSON(string filename){

    ifstream i(filename);
    json j;
    i >> j;

    Topology top(j["id"]);
    for(auto& iter: j["components"]) {
        string properties_key;
        for (auto& el : iter.items()) {
            if(el.key() != "id" && el.key() != "type" && el.key() != "netlist"){
                properties_key = el.key();
                // cout << properties_key << endl;
            }
        };
        Device d(iter["id"], iter[properties_key], iter["netlist"], iter["type"], properties_key);
        top.AddDevice(d);
        // d.deviceDetails();
        // cout << d.inNetList("vss")<<endl;
    }
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
    cout << mem.getTopologyDevices("top1") << endl;


}