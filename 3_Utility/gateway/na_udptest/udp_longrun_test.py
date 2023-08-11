import os, sys
import time
import re
# Log path
udp_rx_log = "udptest.log"
udp_tx_log = "udpclient.log"
# Command
CMD_NC = ' | timeout 3 nc -u -q 2 -p 53000 10.42.0.146 53002'
CMD_SET_UDPIP = 'echo "setudpip 10 42 0 28"' + CMD_NC
CMD_SET_AURIXCAN = 'echo "caninit 500000"' + CMD_NC
CMD_DISABLE_CANLOG = 'echo "canlog off"' + CMD_NC
CMD_START_UDP_SERVER = './udp_qserver &'
CMD_SEND_UDP = './udp_client_can XA AA a 1 8 '
# Parameter
SEND_DATA = [0, 0]
CHECK_DATA = []
MAXIUM_4BYTES = 4294967295 #0xFFFFFFFF
LOOP = 20

def setData():
    SEND_DATA[1] += 1
    if SEND_DATA[1] >= MAXIUM_4BYTES:
        SEND_DATA[1] = 0
        SEND_DATA[0]+= 1
    result = SEND_DATA[0]*(MAXIUM_4BYTES+1) + SEND_DATA[1]
    CHECK_DATA.append(result)

def checkData():
    # 1. Receive or not?
    loopcnt = 0
    while os.path.getsize(udp_rx_log) == 0:
        loopcnt += 1
        print('wait %d'%loopcnt)
        time.sleep(0.1)
        if loopcnt >= 5:
            print('Failed! No data received.')
            return False
    
    # Read
    f = open(udp_rx_log, 'r')

    # Check in loop
    for index in range(LOOP):
        line_cout = f.readline()    # ****** Get UDP frame, Cnt: [1] ******
        line_info = f.readline()    # INFO: 001D
        line_node = f.readline()    # CAN Bus ID: Node_B
        line_mgid = f.readline()    # Message ID: 1
        line_temp = f.readline()    # Payload Data:
        line_data = f.readline()    # 0x00 0x00 0x00 0x00 0x01 0x00 0x00 0x00

        if re.search(r'[[](\d+)[]]',line_cout) is not None:
            get_count = re.search(r'[[](\d+)[]]',line_cout)
            get_count = re.search(r'(\d+)',get_count.group())
            count = get_count.group()

        # 2. Check Bus ID
        if "Node_B" not in line_node:
            print('[%s]Failed! Bus_ID error.'%count)
            print(line_node + line_mgid)
            f.close()
            return False
        # 3. Check data
        can_data = line_data
        can_data = can_data.replace('0x', '')
        temp_list = can_data.split(' ')
        atype = ""
        for i in range(3, -1, -1):
            atype += temp_list[i]
        btype = ""
        for i in range(7, 3, -1):
            btype += temp_list[i]
        can_data = atype + btype
        can_data = int(can_data, 16)
        verify_data = CHECK_DATA[index]
        if can_data == verify_data:
            pass
        else:
            print('[%s]Failed! Data error.'%count)
            print("Verify : %d, DATA: %d"%(verify_data, can_data))
            print(line_temp + line_data)
            f.close()
            return False
    # Pass
    f.close()
    return True

def clearLog():
    f = open(udp_rx_log, 'w')
    f.close()
    del CHECK_DATA[:]

def now():
    return time.time()

def main():
    # set Aurix
    os.system(CMD_SET_UDPIP)
    os.system(CMD_SET_AURIXCAN)
    os.system(CMD_DISABLE_CANLOG)
    # start server
    os.system(CMD_START_UDP_SERVER)
    sys_cnt = 0
    while 1:
        sys_cnt += 1
        # set timer
        start = now()
        # clear log
        clearLog()
        time.sleep(0.1)
        # Send loop
        for i in range(LOOP):
            # set data
            setData()
            # send data
            send_string = CMD_SEND_UDP + '%d %d > '%(SEND_DATA[0], SEND_DATA[1]) + udp_tx_log
            os.system(send_string)
        # wait log data is done
        time.sleep(0.5)
        # check data
        if checkData() is False:
            sys.exit(1)
        else:
            # wait until 1 second
            while (now() - start) < 1:
                pass
            print("UDP_LOOP[%d] (Cost time:%.06f) PASS!"%(sys_cnt,(now() - start)))

if __name__ == '__main__':
    main()
    sys.exit(0)