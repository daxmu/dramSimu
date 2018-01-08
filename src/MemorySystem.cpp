#include "MemorySystem.h"

void MemorySystem::init(MasterPort* masterPort){
	cout << "MemorySystem init begin..." << endl;
	
	// routePort init
	vector<Port*> channelInPort;
	for(auto i = channels.begin(); i < channels.end(); i++){
		channelInPort.push_back((*i).get_inPort());
	}
	routePort.init(masterPort, channelInPort);

	// channels init
	vector<MasterPort*> routeOutPort = routePort.get_outPort();
	auto j = routeOutPort.begin();
	for(auto i = channels.begin(); (i < channels.end()) && (j < routeOutPort.end()); i++){
		(*i).init(*j);
		j++;
	}

	cout << "MemorySystem init ok!" << endl << endl;
}

void MemorySystem::print_summary(std::size_t cycles){
	for(auto i = channels.begin(); i < channels.end(); i++){
		(*i).print_summary(cycles);
	}
}
void MemorySystem::run_step(){
	routePort.run_step();
	for(auto i = channels.begin(); i < channels.end(); i++)
		(*i).run_step();
}

void MemorySystem::update(){
	routePort.update();
	for(auto i = channels.begin(); i < channels.end(); i++)
		(*i).update();
}
