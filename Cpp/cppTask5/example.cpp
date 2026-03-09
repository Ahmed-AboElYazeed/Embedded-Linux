#include <iostream>
#include <string>


class Stream
{
private:
    std::string pathName;
public:
    Stream(std::string&& path){
        std::cout<<"from Stream\n";
        pathName = path;
    }
    ~Stream(){
        std::cout << "Stream Desctructor \n";
    }
    // virtual void open() = 0;
};


class InStream: virtual public Stream
{
private:
    std::istream& in;
public:
    InStream(std::string&& path, std::istream& in_stream): Stream(std::move(path)), in(in_stream){
        std::cout<<"from InStream\n";
    }
    void open(){
        std::cout<<"open from InStream\n";
    }
    std::istream& operator >>(std::string& inputString)
    {
        std::cout << "Please write the string\n";
        in >> inputString;
        /*write on fd*/
        return in;
    }
    ~InStream(){
        std::cout << "InStream Desctructor \n";
    }
};

class OutStream: virtual public Stream
{
private:
    std::ostream& out;
public:
    OutStream(std::string&& path, std::ostream& out_stream): Stream(std::move(path)), out(out_stream){
        std::cout<<"from OutStream\n";
    }

    // void open(){
    //     std::cout<<"open from OutStream\n";
    // }
    std::ostream& operator <<(std::string& outstring)
    {
        /*read from fd the contents and save it in outstring */
        out << outstring;
        return out;
    }
    ~OutStream(){
        std::cout << "Outstrem Desctructor \n";
    }
};

class IOStream: public OutStream, public InStream {

    
    public:
        IOStream(std::string&& path, std::ostream& out_stream, std::istream& in_stream) : 
                                    InStream(std::move(path), in_stream), 
                                        OutStream(std::move(path), out_stream),
                                            Stream(std::move(path))
        {
            std::cout << "IOstream Constructor \n";
        }

        ~IOStream(){
            std::cout << "IOstream Desctructor \n";
        }
};

int main(int argc, char const *argv[])
{
    std::string inputString;
    IOStream inout ({"/home/maxmaster/data/ITI/ITI-ES/lab_10/file"}, std::cout, std::cin);
    inout >> inputString;
    inout << inputString << std::endl;

    inout.open();    // only when one of the middle class is implemnting it, (work with and without pure virtual)
    // inout.InStream::open(); //correct  without using virtual pure in the base class

}

