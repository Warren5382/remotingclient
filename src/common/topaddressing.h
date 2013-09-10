#ifndef TOPADDRESSING_H
#define TOPADDRESSING_H

class TopAddressing {
private:
	std::string nsAddr;
	static std::string& clearNewLine(std::string& str);
public:
	TopAddressing() {};
	std::string& fetchNSAddr() {};
	std::string getNSAddr(){return nsAddr};
	void setNSAddr(std::string nsAddr) {
		this->nsAddr = nsAddr;
	};

};
#endif