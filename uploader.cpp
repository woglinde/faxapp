#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/http_file.h>
#include <iostream>
#include "content.h"

using namespace std;
class uploader: public cppcms::application {
public:
    uploader(cppcms::service &s) :
        cppcms::application(s) 
    {
    }

    // gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=cfax -sOutputFile=fax.ssf -f input.pdf
    // capifax -v -i "hfcsusb rocks"  -h "hfcsusb rocks" -c 555-9124100
    void main(std::string /*unused*/)
    {
        content::upload c;
        if(request().request_method()=="POST") {
            c.info.load(context());
            if(c.info.validate()) {
                c.senderId = c.info.senderId.value();
                c.headLine = c.info.headLine.value();
                c.senderNumber = c.info.senderNumber.value();
                c.callNumber = c.info.callNumber.value();
                // Create file name
                //
                // Note:
                // NEVER, NEVER, NEVER use user supplied file name!
                //
                // Use it to display or for general information only.
                //
                // If you would try to save the file under user supplied name you
                // may open yourself to multiple security issues like directory
                // traversal and more.
                //
                // So create your own name. If you want to keep the connection with original
                // name you may use sha1 hash and then save it.
                //
                std::string new_name = "/tmp/fax.pdf";
                //
                // Note: save_to is more efficient then reading file from
                // c.info.image.value()->data() stream and writing it
                // as save to would try to move the saved file over file-system
                // and it would be more efficient.
                //
                c.info.faxFile.value()->save_to(new_name);
                c.info.clear();
                std::string gs_command;
                gs_command = "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=cfax -sOutputFile=/tmp/fax.ssf -f";
                gs_command += new_name;
                int error_system, error_remove;
                error_system = system(gs_command.c_str());
                c.error_system = error_system;
                if (error_system == 0) {
                        error_remove = remove(new_name.c_str());
                        c.error_remove = error_remove;
                        if (error_remove != 0) {
                                cerr << "Could not remove " << new_name  << " errorcode is " << error_remove << endl;
                        }
                } else {
                        cerr << "Could not convert pdf to sff error code is " << error_system << endl;
                }

            }
        }
        render("upload", c);
    }
};

int main(int argc,char ** argv)
{
    try {
        cppcms::service app(argc,argv);
        app.applications_pool().mount(cppcms::applications_factory<uploader>());
        app.run();
    }
    catch(std::exception const &e) {
        cerr << e.what() << endl;
    }
}

// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
