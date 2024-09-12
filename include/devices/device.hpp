#ifndef DEVICE_H
#define DEVICE_H

class Device
{
private:

    uint8_t nodeId = 8;
    
public:

    virtual void init()=0; //funkcje ktore nazucaja potomka koniecznosc ich implementacji
    virtual void cyclic()=0;
    virtual uint16_t getNodeId() {return nodeId;};
   
};



#endif