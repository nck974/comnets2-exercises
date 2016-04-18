simple Reciever
{
    gates:
        input in;
}

simple Sender {
  parameters:
    volatile double delayTime @unit(s);
  gates:
    output out;
}

network Ex2Network
{
    submodules:
        sender: Sender;
        reciever: Reciever;
    connections:
        sender.out --> {  delay = 100ms; } --> reciever.in;
}
