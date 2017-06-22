#include "../include/chrisgraph.h"
#include "../../heap/include/heap.h"
#include "../../heap/include/nheap.h"
#include "../blossomv/PerfectMatching.h"
#include <cmath>

using namespace std;

namespace Christofides{
    
    Distance ChrisGraph::dist(unsigned int i, unsigned int j) const{
            double xd, yd, zd;
            double q1, q2, q3;
            const double RRR = 6378.388;
            switch(this->dtype){
                case EUC_2D:
                    xd = this->px[i] - this->px[j];
		            yd = this->py[i] - this->py[j];
		            return lrint(sqrt(xd*xd+yd*yd));
		        case EUC_3D:
		            xd = this->px[i] - this->px[j];
		            yd = this->py[i] - this->py[j];
		            zd = this->pz[i] - this->pz[j];
		            return lrint(sqrt(xd*xd+yd*yd+zd*zd));
		        case MAX_2D:
		            xd = this->px[i] - this->px[j];
		            yd = this->py[i] - this->py[j];
		            return std::max(lrint(xd), lrint(yd));
		        case MAX_3D:
		            xd = this->px[i] - this->px[j];
		            yd = this->py[i] - this->py[j];
		            zd = this->pz[i] - this->pz[j];
		            return std::max(lrint(zd), std::max(lrint(xd), lrint(yd)));
		        case MAN_2D:
		       		xd = abs(this->px[i] - this->px[j]);
		            yd = abs(this->py[i] - this->py[j]);
		            return lrint(xd+yd);            
		        case MAN_3D:
		       		xd = abs(this->px[i] - this->px[j]);
		            yd = abs(this->py[i] - this->py[j]);
		            zd = abs(this->pz[i] - this->pz[j]);
		            return lrint(xd+yd+zd);            
		        case CEIL_2D:
		            xd = this->px[i] - this->px[j];
		            yd = this->py[i] - this->py[j];
		            return Distance(ceil(sqrt(xd*xd+yd*yd)));
		        case GEO:
		            q1 = cos(this->py[i] - this->py[j]);
		            q2 = cos(this->px[i] - this->px[j]);
		            q3 = cos(this->px[i] + this->px[j]);
		            return Distance(RRR * acos(0.5*((1.0+q1)*q2-(1.0-q1)*q3))+1.0);
		        default:
		            throw std::string("Invalid distance type ")+this->type+std::string(" \n");
        }
    }
    
    
    //read keywords from the stream (separated by spaces)
    std::string read_keyword(std::istream& f) {
        std::string keyword;
        if (f >> keyword)
            return keyword;
        else
            throw "EOF";
    }
    
    //skip the separator : in the stream
    void skip_separator(std::istream& f) {
        std::string sep;
        f >> sep;
        if (sep != ":") //separator is a ":"
            throw "Expected separator : but found \'"+sep+"\'";
    }
    
    
    //convert x,y input to lat,long input
    std::pair<double,double> latlong(double x, double y){
        double deg = lrint(x);
        double min = x - deg;
        double lat = M_PI * (deg + 5.0 * min / 3.0 ) / 180.0;
        deg = lrint(y);
        min = y - deg;
        double lon = M_PI * (deg + 5.0 * min / 3.0 ) / 180.0; 
        
        return std::make_pair(lat, lon);
    }
    
       
    //read the specification area of the input stream
    unsigned int read_specification(ChrisGraph& g, std::istream& f){
        std::string keyword;
        g.node_coord_type = "TWOD_COORDS";
        
        do {
            keyword = read_keyword(f);
            if (keyword[keyword.length()-1] == ':') {
                keyword = keyword.substr(0,keyword.length()-1);
                f.putback(':');
            }   
    
            if(keyword == "NAME"){
                skip_separator(f);
                f >> g.name;
            }else if(keyword == "TYPE"){
                skip_separator(f);
                f >> g.type;
                if( g.type != "TSP"&& g.type != "ATSP" && g.type != "SOP" && g.type != "HCP" && g.type != "CVRP" && g.type != "TOUR"){
                    throw "Unknown instance type \""+g.type+"\"\n";
                    return -1;
                }
            }else if(keyword == "COMMENT"){
                skip_separator(f);
                char cline[1024];
                f.getline(cline,1024);
                g.comment = cline;
            }else if(keyword == "DIMENSION"){
                skip_separator(f);
                f >> g.dim;
            }else if(keyword == "CAPACITY"){
                skip_separator(f);
                unsigned int cap;
                f >> cap;
                throw "Found capacity. What to do?\n";
                return -1;
            }else if(keyword == "EDGE_WEIGHT_TYPE"){
                skip_separator(f);
                f >> g.edge_weight_type;
                if(g.edge_weight_type == "EUC_2D")
                    g.dtype = EUC_2D;
                else if(g.edge_weight_type == "EUC_3D")
                    g.dtype = EUC_3D;
                else if(g.edge_weight_type == "MAX_2D")
                    g.dtype = MAX_2D;
                else if(g.edge_weight_type == "MAX_3D")
                    g.dtype = MAX_3D;
                else if(g.edge_weight_type == "MAN_2D")
                    g.dtype = MAX_2D;
                else if(g.edge_weight_type == "MAN_3D")
                    g.dtype = MAN_3D;
                else if(g.edge_weight_type == "CEIL_2D")
                    g.dtype = CEIL_2D;
                else if(g.edge_weight_type == "GEO")
                    g.dtype = GEO;    
                else{
                    throw std::string("Unkown edge weight type ")+g.edge_weight_type;
                    return -1;
                }
            }else if(keyword == "EDGE_WEIGHT_FORMAT"){
                skip_separator(f);
                f >> g.edge_weight_format;
            }else if(keyword == "EDGE_DATA_FORMAT"){
                skip_separator(f);
                f >> g.edge_data_format;
            }else if(keyword == "NODE_COORD_TYPE"){
                skip_separator(f);
                f >> g.node_coord_type;
            }else if(keyword == "DISPLAY_DATA_TYPE"){
                skip_separator(f);
                f >> g.display_data_type;
            }else if (keyword == "EOF"){
                skip_separator(f);
            }else {
              return 0;
            }
        } while (true);
    }
       
    //read the data part of the input stream
    unsigned int read_data(ChrisGraph& g, std::istream& f){
        //allocate memory
        g.px.assign(g.dim, 0.0);
        g.py.assign(g.dim, 0.0);
        if(g.node_coord_type == "THREED_COORDS")
            g.pz.assign(g.dim, 0.0);
            
        std::string keyword = "NODE_COORD_SECTION";
        
        try {
            do {
                if (keyword == "NODE_COORD_SECTION") {
	                if (g.node_coord_type == "TWOD_COORDS") {
	                    // get coordinates
	                    for(unsigned int i=0; i<g.dim; i++) {
	                        unsigned int j;
	                        double x,y;
	                        f >> j >> x >> y;
	                        if(g.dtype != GEO){
	                            g.px[j-1] = x;
	                            g.py[j-1] = y;
	                        }else{
	                            std::pair<double,double> ll = latlong(x,y);
	                            g.px[j-1] = ll.first;
	                            g.py[j-1] = ll.second;
	                        } 
	                    }                   	                
	                }else if (g.node_coord_type == "THREED_COORDS") {
	                    // get coordinates
	                    for(unsigned int i=0; i<g.dim; i++) {
	                        unsigned int j;
	                        double x,y,z;
	                        f >> j >> x >> y >> z;
                            g.px[j-1] = x;
	                        g.py[j-1] = y;
	                        g.pz[j-1] = z;
	                    }                   	                
	                }else 
	                    throw std::string("Unsupported coordinate type ")+g.node_coord_type;
                }else if (keyword == "DEPOT_SECTION") {
                	throw std::string("Unsupported data section ")+keyword;
                }else if (keyword == "DEMAND_SECTION") {
	                throw std::string("Unsupported data section ")+keyword;
                }else if (keyword == "EDGE_DATA_SECTION") {
	                throw std::string("Unsupported data section ")+keyword;
                }else if (keyword == "FIXED_EDGES_SECTION") {
	                throw std::string("Unsupported data section ")+keyword;
                }else if (keyword == "DISPLAY_DATA_SECTION") {
	                throw std::string("Unsupported data section ")+keyword;
                }else if (keyword == "TOUR_SECTION") {
	                throw std::string("Unsupported data section ")+keyword;
                }else if (keyword == "EDGE_WEIGHT_SECTION") {
		            throw std::string("Unsupported data section ")+keyword;    
                }else if (keyword == "EOF") {
                	return 0;
                }else if (keyword == "" && f.eof()) {
	                return 0;
                }else
                    throw "Unknown keyword \""+keyword+"\"";
                    
                keyword = read_keyword(f);
            }while (true);
        }catch (const std::string& s) {
            std::cout << s << std::endl;
            return -1;
        }catch (...) {
            std::cout << "Warning: Unexpected end of file while scanning for keyword." << std::endl;
            return -1;
        }
    }
    
    //read the input stream into a graph for chirstofides algorithm    
    unsigned int read_instance(ChrisGraph& g, std::istream& f){
        try{
            if(read_specification(g, f) != 0){
                std::cout << "Error reading specification of input file\n" << std::endl;
                return -1;
            }
            
            if(read_data(g, f) != 0){
                std::cout << "Error reading data of input file\n" << std::endl;
                return -1;
            }
        }catch(const std::string& s){
            std::cout << s << std::endl;
            return -1;
        }
        
        return 0;
    }
    
    //find a MST in the graph
    MST findMST(const ChrisGraph& g){
        std::vector<bool> visited(g.dim, false);
        std::vector<unsigned int> prev(g.dim, MAX_WEIGHT); //the previous of each node in the MST construction
        std::vector<Distance> weight(g.dim, MAX_WEIGHT);
        
        NHeap h(g.dim, 4);
        weight[0] = 0; //starting at node 0 (graph is complete, does not make much difference)
        h.insert(0, weight[0]);
        for(unsigned int u=1;u<g.dim;u++)
            h.insert(u,weight[u]);
        
        while(not h.is_empty()){
            unsigned int u = h.gettop();
            h.deletetop();
            visited[u] = true;
            for(unsigned int v=0;v<g.dim;v++){                
                if(not visited[v] && g.dist(u,v) < weight[v]){
                    h.update_key(v, g.dist(u,v));
                    prev[v] = u;
                }
            }
        }

        MST mt(g.dim);
        Distance cost = 0;
                
        for(unsigned int u=1;u<g.dim;u++){
            if(prev[u] == MAX_WEIGHT){
                std::cout << "An error occurred during MST creation " << std::endl;
                exit(-1);
            }
            mt[u].push_back(prev[u]);
            mt[prev[u]].push_back(u);    
            cost += g.dist(u, prev[u]);
        }
        
        std::cout << "MST cost = " << cost << std::endl;
        
        return mt;
    }

    //find a matching in the mst generated from the input graph
    void findMatching(const MST& mt, const ChrisGraph& g){
        std::vector<unsigned int> oddn; // nodes with odd number of neighbors
        for(unsigned int u=0;u<mt.size();u++){
            if(mt[u].size() % 2 != 0)
                oddn.push_back(u);
        }
        
        PerfectMatching pm(oddn.size(), oddn.size()*oddn.size());
        for(unsigned int u=0;u<oddn.size();u++)
            for(unsigned int v=u+1;v<oddn.size();v++)
                pm.AddEdge(u,v,g.dist(u,v));
                
        pm.Solve();
        
        std::cout << "There are " << oddn.size() << " nodes of odd degree\n";
    }    
    
    //runs christofides algorithm to get an approximation of a TSP solution
    Distance chris_algorithm(const ChrisGraph& g){
        MST mt = findMST(g);
        
        findMatching(mt, g);
        
        return 0;
    }
  
    
}