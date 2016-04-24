simple Reciever
{
    parameters:
      @display("i=block/sink");
    gates:
      input in;
}

simple Sender {
  parameters:
    @display("i=block/source");
    volatile double delayTime @unit(s);
  gates:
    output out;
}

network Ex3Network
{
    submodules:
        sender: Sender;
        reciever: Reciever;
    connections:
        sender.out --> {  delay = 100ms; } --> reciever.in;
}
