#include "../inc/can.h"

int open_canbus_socket(const char *ifname)
{

	uint8_t fd;
	int ret;
	struct timeval tv;
	struct sockaddr_can addr;
	struct ifreq ifr;
	
	if((fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
		log_msg(LOG_ERR, STD_ERR, "Fail to create CAN Socket");
		ret = CAN_OPEN_SOC_ERROR;
		return ret;
	}
	else
	{
		strcpy(ifr.ifr_name, ifname);
		ioctl(fd, SIOCGIFINDEX, &ifr);
		addr.can_family  = AF_CAN;
		addr.can_ifindex = ifr.ifr_ifindex;
		
		log_msg(LOG_DEBUG, STD_OUT, "%s at index %d", ifname, ifr.ifr_ifindex);
		
		if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{ 
			log_msg(LOG_ERR, STD_ERR, "CAN socket Bind error");
			ret = CAN_OPEN_SOC_BIND_ERROR;
			return ret;
		}
		else
		{
			tv.tv_sec = 0;
			//can-bus read time-out issue, decrease the can-bus set time-out
			tv.tv_usec = 2000; 
			//CAN_TIMEOUT_SET_FAIL = -22;
			if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
			{
			     log_msg(LOG_ERR, STD_ERR, "set socket options error");
			     ret = CAN_SOC_SET_TIMEOUT_ERROR;
			     return ret;
			}
		}
	}
	ret = fd;
	log_msg(LOG_INFO, STD_OUT, "CAN socket initialization is success!");
	return fd;
}

int close_canbus_socket(char fd)
{
	int ret;

	if (fd > 0)
	{
	  if (close(fd) < 0)
	  {
	      log_msg(LOG_ERR, STD_ERR, "Fail to close socket fd = %d", fd);
	      ret = CAN_CLOSE_ERROR;
	  }
	  else 
	  {
	      log_msg(LOG_INFO, STD_OUT, "Successfully closed CAN socket fd = %d", fd);
	      ret = CAN_SUCCESS;
	  }
	}
	else
	{
		log_msg(LOG_ERR, STD_OUT, "Invalid CAN socket fd %d", fd);
		ret = CAN_CLOSE_FD_ALREADY_CLOSED;
	}

	return ret;
}

int write_canbus_frame(char fd, struct can_frame frame)
{
	int ret;
	int nbytes = 0;
	
	//write the buffer
	nbytes = write(fd, &frame, sizeof(struct can_frame));
	if (nbytes < 1)
	{
		ret = CAN_WRITE_ERROR;
		log_msg(LOG_ERR, STD_ERR, "Error in writing CAN-Frame, nbytes: %d", nbytes);
	}
	else 
	{
		log_msg(LOG_DEBUG, STD_OUT, "Written %d bytes on CAN-BUS", nbytes);
		ret = CAN_SUCCESS;
	}
	
	return ret;
}

int read_canbus_frame(char fd, struct can_frame *frame)
{
	int ret = 0;
	int nbytes = 0;
	
	nbytes = read(fd, frame, sizeof(struct can_frame));
	if(nbytes < 2)
	{
		log_msg(LOG_DEBUG, STD_OUT, "%s failed to read CAN frame", __func__);
		ret = CAN_READ_ERROR;
	}
	else
	{
		log_msg(LOG_INFO, STD_OUT, "%d bytes read from the CAN-BUS", nbytes);
		ret =  nbytes;
	}
	
	return ret;
}
