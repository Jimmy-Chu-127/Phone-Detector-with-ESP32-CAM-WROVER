from ECE16Lib.Communication import Communication

if __name__ == "__main__":
    # comms = Communication("COM11", 115200)  #HC Comms
    comms = Communication("COM7", 115200)
    comms.clear()

    try:
        while(True):
            message = comms.receive_message()
            if(message != None):
                print(message)

    except(Exception, KeyboardInterrupt) as e:
        print(e)
    finally:
        print("Closing connection.")
        comms.close()