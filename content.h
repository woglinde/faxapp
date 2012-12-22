#ifndef CONTENT_H
#define CONTENT_H

#include <cppcms/view.h>
#include <cppcms/form.h>

#include <iostream>


namespace content  {
 
struct file_form : public cppcms::form {
    cppcms::widgets::text senderId;
    cppcms::widgets::text headLine;
    cppcms::widgets::text senderNumber;
    cppcms::widgets::text callNumber;
    cppcms::widgets::file faxFile;
    cppcms::widgets::submit submit;
    file_form()
    {
        senderId.message("Sender Id");
        senderId.non_empty();
        headLine.message("Head Line");
        headLine.non_empty();
        senderNumber.message("Number of sender");
        senderNumber.non_empty();
        callNumber.message("Number to fax");
        callNumber.non_empty();
        faxFile.message("PDF to fax");
        faxFile.help("PDF's will be converted to ssf-files and than fax");
        faxFile.non_empty();

        // Sanity checks on the pdf

        // Filename with proper extension (optional)
        faxFile.filename(booster::regex(".*\\.pdf"));
        // Correct mime type (optional)
        faxFile.mime(booster::regex("application/pdf"));

        // And the most important (more then two above) the
        // magic numbers for each file and this the most important
        // validation
        //
        // Note: this what really defines the file type, other can be
        // easily changed
        //pdf hex
        faxFile.add_valid_magic("\x25\x50\x44\x46");

        submit.value("fax it");

        add(senderId);
        add(headLine);
        add(senderNumber);
        add(callNumber);
        add(faxFile);
        add(submit);
    }
};

struct upload : public cppcms::base_content {
    std::string senderId, headLine, senderNumber, callNumber;
    int error_system, error_remove;
    file_form info;
};

} // content


#endif
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
