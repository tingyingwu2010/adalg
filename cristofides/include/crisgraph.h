#ifndef __CRISGRAPH_H__
#define __CRISGRAPH_H__

#include <vector>
#include <iostream>
#include <string>
#include <math.h>

typedef unsigned Distance;

namespace Cristofides{

    enum DistType{ EUC_2D = 1024, EUC_3D, MAX_2D, MAX_3D, MAN_2D, MAN_3D,
                     CEIL_2D};

    struct CrisGraph{
        std::vector<double> px;
        std::vector<double> py;
        //std::vector<double> pz; //TODO: check if 3D graphs are used
        
        DistType dtype; //distance type of the graph
        std::string name, type, comment, edge_weight_type, edge_weight_format, 
                           edge_data_format, node_coord_type, display_data_type;
                           
        unsigned int dim; //dimension of the problem (num nodes)
        
        
        Distance dist(unsigned int i, unsigned int j){
            double xd, yd, zd;
            switch(this->dtype){
                case EUC_2D:
                    xd = this->px[i] - this->px[j];
		            yd = this->py[i] - this->py[j];
		            return lrint(sqrt(xd*xd+yd*yd));
		        case EUC_3D:
		            xd = this->px[i] - this->px[j];
		            yd = this->py[i] - this->py[j];
		            zd = 0;//this->pz[i] - this->pz[j];
		            return lrint(sqrt(xd*xd+yd*yd+zd*zd));
		        case MAX_2D:
		            xd = this->px[i] - this->px[j];
		            yd = this->py[i] - this->py[j];
		            return std::max(lrint(xd), lrint(yd));
		        case MAX_3D:
		            xd = this->px[i] - this->px[j];
		            yd = this->py[i] - this->py[j];
		            zd = 0;//this->pz[i] - this->pz[j];
		            return std::max(lrint(zd), std::max(lrint(xd), lrint(yd)));
		        case MAN_2D:
		       		xd = abs(this->px[i] - this->px[j]);
		            yd = abs(this->py[i] - this->py[j]);
		            return lrint(xd+yd);            
		        case MAN_3D:
		       		xd = abs(this->px[i] - this->px[j]);
		            yd = abs(this->py[i] - this->py[j]);
		            zd = 0;//abs(this->pz[i] - this->pz[j]);
		            return lrint(xd+yd+zd);            
		        case CEIL_2D:
		            xd = this->px[i] - this->px[j];
		            yd = this->py[i] - this->py[j];
		            return Distance(ceil(sqrt(xd*xd+yd*yd)));
		        default:
		            throw std::string("Invalid distance type ")+this->type+std::string(" \n");
            }
        }
        
    };

    std::string read_keyword(std::istream& f) {
        std::string keyword;
        if (f >> keyword)
            return keyword;
        else
            throw "EOF";
    }

    void skip_separator(std::istream& f, const std::string& keyword) {
        std::string sep;
        f >> sep;
        if (sep != ":") //separator is a ":"
            throw "Expected separator : but found \'"+sep+"\'";
    }
    
    
    unsigned int read_specification(CrisGraph& g, std::istream& f){
        std::string keyword;
        g.node_coord_type = "TWOD_COORDS";
        do {
            keyword = read_keyword(f);
            if (keyword[keyword.length()-1] == ':') {
                keyword = keyword.substr(0,keyword.length()-1);
                f.putback(':');
            }   
    
            if(keyword == "NAME"){
                skip_separator(f,keyword);
                f >> g.name;
            }else if(keyword == "TYPE"){
                skip_separator(f,keyword);
                f >> g.type;
                if( g.type != "TSP"&& g.type != "ATSP" && g.type != "SOP" && g.type != "HCP" && g.type != "CVRP" && g.type != "TOUR"){
                    throw "Unknown instance type \""+g.type+"\"\n";
                    return -1;
                }
            }else if(keyword == "COMMENT"){
                skip_separator(f,keyword);
                char cline[1024];
                f.getline(cline,1024);
                g.comment = cline;
            }else if(keyword == "DIMENSION"){
                skip_separator(f,keyword);
                f >> g.dim;
            }else if(keyword == "CAPACITY"){
                skip_separator(f,keyword);
                unsigned int cap;
                f >> cap;
                throw "Found capacity. What to do?\n";
                return -1;
            }else if(keyword == "EDGE_WEIGHT_TYPE"){
                //TODO: add geo type
                skip_separator(f,keyword);
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
                else{
                    throw "Unkown edge weight type "+g.edge_weight_type+" \n";
                    return -1;
                }
            }else if(keyword == "EDGE_WEIGHT_FORMAT"){
                skip_separator(f,keyword);
                f >> g.edge_weight_format;
            }else if(keyword == "EDGE_DATA_FORMAT"){
                skip_separator(f,keyword);
                f >> g.edge_data_format;
            }else if(keyword == "NODE_COORD_TYPE"){
                skip_separator(f,keyword);
                f >> g.node_coord_type;
            }else if(keyword == "DISPLAY_DATA_TYPE"){
                skip_separator(f,keyword);
                f >> g.display_data_type;
            }else if (keyword == "EOF"){
                skip_separator(f,keyword);
            }else {
              return 0;
            }
        } while (true);
    }
    
    unsigned int read_data(CrisGraph& g, std::istream& f){
        //allocate memory
        g.px.assign(g.dim, 0.0);
        g.py.assign(g.dim, 0.0);
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
	                        g.px[j-1] = x;
	                        g.py[j-1] = y;
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
        
    unsigned int read(CrisGraph& g, std::istream& f){
        if(read_specification(g, f) != 0){
            std::cout << "Error reading specification of input file\n" << std::endl;
            return -1;
        }
        
        if(read_data(g, f) != 0){
            std::cout << "Error reading data of input file\n" << std::endl;
            return -1;
        }
        
        return 0;
    }

}

#endif //__CRISGRAPH_H__