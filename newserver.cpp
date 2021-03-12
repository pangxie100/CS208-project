#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "boost/serialization/serialization.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include <boost/serialization/export.hpp>
#include "boost/foreach.hpp"
#include "boost/any.hpp"
#include <boost/serialization/vector.hpp>

#include <string>
//#include <Windows.h>
#include <iostream>
#include <sstream>
#include <vector>

#define DEFAULT_PORT 8000
#define MAXLIN 4096

class CMyData
{
private:
	friend class boost::serialization::access;
 
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & _tag;
		ar & _text;
	}
 
	
public:
	CMyData():_tag(0), _text(""){}
 
	CMyData(int tag, std::string text):_tag(tag), _text(text){}
	virtual ~CMyData(){}
 
	int GetTag() const {return _tag;}
	std::string GetText() const {return _text;}
 
private:
	int _tag;
	std::string _text;
};

class CMyData_Child: public CMyData
{
private:
	friend class boost::serialization::access;
 
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		// serialize base class information
		ar & boost::serialization::base_object<CMyData>(*this);
		ar & _number;
	}
 
 
public:
	CMyData_Child():_number(0.0){}
 
	CMyData_Child(int tag, std::string text, float number):CMyData(tag, text), _number(number){}
 
	float GetNumber() const{return _number;}
 
private:
	float _number;
};
 
BOOST_CLASS_EXPORT_GUID(CMyData_Child, "CMyData_Child") 

class CMyData_ContainerSTL
{
private:
	friend class boost::serialization::access;
 
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & vPointers;
	}
public:
	std::vector<CMyData*> vPointers;
};

void TestPointerArchiveWithSTLCollections()
{
	std::string content;
	{
		CMyData d1(1, "parent obj");
		CMyData_Child d2(2, "child obj", 2.5);
 
		CMyData_ContainerSTL containter;
		containter.vPointers.push_back(&d1);
		containter.vPointers.push_back(&d2);
		containter.vPointers.push_back(&d1);
		
 
		std::ostringstream os;
		boost::archive::binary_oarchive oa(os);
		oa << containter;
 
		content = os.str();
	}
 
	//反序列化
	{
		CMyData_ContainerSTL container;
		std::istringstream is(content);
		boost::archive::binary_iarchive ia(is);
		ia >> container;
 
		std::cout<<"Test STL collections:\n";
		BOOST_FOREACH(CMyData* p, container.vPointers)
		{
			std::cout << "object text: " << p->GetText() << "\n";
 
			CMyData_Child* child = dynamic_cast<CMyData_Child*>(p);
			if (child)
			{
				std::cout << "child object number: " << child->GetNumber() << "\n";
			}
		}
	}
}

int sendall(int sockfd, const char *buf, int size);
 
int main(int argc,char **argv)
{
    int socket_fd,connect_fd;
    struct sockaddr_in servaddr;
    char buff[4096];
    int n;
    int len;

    TestPointerArchiveWithSTLCollections();
    // serialization
    CMyData d1(1, "parent obj");
    CMyData_Child d2(2, "child obj", 2.5);

    CMyData_ContainerSTL containter;
    containter.vPointers.push_back(&d1);
    containter.vPointers.push_back(&d2);
    containter.vPointers.push_back(&d1);
    

    std::ostringstream os;
    boost::archive::binary_oarchive oa(os);
    oa << containter;

    std::string content;
    content = os.str();
    std::cout << "content size = " << content.size() << std::endl;
    std::ostringstream oss;
    oss << "String";
    std::cout << "oss size = " << oss.tellp() << std::endl;
    std::cout << "os size = " << os.tellp() << std::endl;
    int str_len = os.tellp();
    std::cout << "str_len = " << str_len << std::endl;
    double num = 1520621272289939195.000000;
    std::cout << "num = " << num << std::endl;
    std::cout << "num = " << (int)num << std::endl;
    std::cout << "num = " << (long int)num << std::endl;
    // serialization

    // Initialization
    if((socket_fd=socket(AF_INET,SOCK_STREAM,0)) == -1) // AF_INET is ipv4, AF_INET6 is ipv6
    {
        printf("create socket error:%s(errno :%d)\n",strerror(errno),errno);
        exit(0);
    }
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    // set IP address to INADDR_ANY, let system get it automatically
    servaddr.sin_port=htons(DEFAULT_PORT);
    // set the port to DEFAULT_PORT (8000)

    // bind local address to the built socket
    if(bind(socket_fd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        printf("bind socket error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);
    }

    // start to listen to client connect, the second parameter is the max number of listen
    if(listen(socket_fd,10) == -1)
    {
        printf("listen socket error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);
        
    }
    printf("======waiting for client's request=====\n");
    while(1){
        if((connect_fd = accept(socket_fd,(struct sockaddr*)NULL,NULL)) == -1){
            printf("accept socket error :%s(errno:%d)\n",strerror(errno),errno);
            exit(1);
        }
        printf("new client connect.\n");
        if(!fork()){
            
            FILE *fpRead = fopen("./dataset-corridor4_512_16/mav0/cam0/data/1520621175986840704.png", "r");
            int image_len = 0;
            if (fpRead == NULL){
                perror("Read file error");
                return 0;
            }

            // for image transportation
            fseek(fpRead, 0, SEEK_END);
            image_len = ftell(fpRead);
            printf("image_len = %d\n", image_len);
            rewind(fpRead);
            char* ImgBuffer=(char*)malloc( image_len * sizeof(char) );
            fread(ImgBuffer, image_len, 1, fpRead);
            //FILE *fpWrite = fopen("mydata_client_copy.png", "w");
            //fwrite(ImgBuffer, image_len, 1, fpWrite);
            //fclose(fpWrite);
            
            if (sendall(connect_fd, ImgBuffer, image_len) == -1){
                perror("send error");
                close(connect_fd);
                exit(0);
            }
            // for image transportation

            printf("file open ends, start to send.\n");

            /*
            // for chars transportation
            while (fgets(buff, MAXLIN, fpRead) != NULL){
                printf("%s", buff);
                len = strlen(buff);
                printf("len of buff = %d\n", len);
                if (sendall(connect_fd, buff, len) == -1){
                    perror("send error");
                    close(connect_fd);
                    exit(0);
                }
                
                //if (send(connect_fd, buff, len, 0) == -1){
                //    perror("send error");
                //    close(connect_fd);
                //    exit(0);
                //}
                
            }
            */
            // for chars transportation
            
            printf("file sending ends.\n");
            printf("\n");
            fclose(fpRead);
            free(ImgBuffer);
        }
        close(connect_fd);
    }
    
    close(socket_fd);
    printf("server socket closed.\n");
    return 0;
}

// when sending too fast, sendlen(res in each time) may < size
// need to send the rest
int sendall(int sockfd, const char *buf, int size)
{
    int sendlen = 0;
    while (sendlen < size) {
        printf("size = %d\n", size);
        int res = send(sockfd, buf + sendlen, size - sendlen, 0);
        printf("res = %d\n", res);
        if(res >= 0) {
            sendlen += res;
            printf("sendlen = %d\n", sendlen);
        } else if( res == -1 ) { 
            if( errno != EINTR && errno != EAGAIN) {
                return -1; 
            }   
        }   
    }   
    return 0;
}