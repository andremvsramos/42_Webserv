#include "../headers/webserv.hpp"

/* ===================== Temporary Directory Listing Function ===================== */

int createListHTML(std::string location, std::ofstream& file) {
    file << "<!DOCTYPE html> \n"
         << "  <html lang=\"en\">\n"
         << "  <head>\n"
         << "      <meta charset=\"UTF-8\">\n"
         << "      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
         << "      <title>Login</title>\n"
         << "      <link rel=\"icon\" type=\"image/x-icon\" href=\"/images/favicon.ico\">\n"
         << "      <style>\n"
         << "          body {\n"
         << "              font-family: Arial, sans-serif;\n"
         << "              margin: 0;\n"
         << "              padding: 0;\n"
         << "              background-color: #ffffff; /* White background */\n"
         << "          }\n"
         << "          \n"
         << "          header {\n"
         << "              background-color: #333;\n"
         << "              color: #fff;\n"
         << "              padding: 10px 0;\n"
         << "              text-align: center;\n"
         << "          }\n"
         << "          \n"
         << "          nav {\n"
         << "              background-color: #444;\n"
         << "              color: #fff;\n"
         << "              padding: 15px 0;\n"
         << "              text-align: center;\n"
         << "              display: flex; /* Align items in a row */\n"
         << "              justify-content: space-between; /* Distribute items with space between them */\n"
         << "          }\n"
         << "          \n"
         << "          nav a {\n"
         << "              color: #fff;\n"
         << "              text-decoration: none;\n"
         << "              padding: 0 15px;\n"
         << "          }\n"
         << "          \n"
         << "          nav a:hover {\n"
         << "              color: #ffd700;\n"
         << "          }\n"
         << "          \n"
         << "          .login-link {\n"
         << "              color: #fff;\n"
         << "              text-decoration: none;\n"
         << "              padding: 0 10px;\n"
         << "              margin-right: 10px; /* Add margin to separate from other links */\n"
         << "          }\n"
         << "\n"
         << "          section {\n"
         << "              padding: 20px;\n"
         << "              margin: 20px;\n"
         << "              background-color: #fff; /* White background */\n"
         << "              border-radius: 5px;\n"
         << "              box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); /* Adding a slight shadow */\n"
         << "              width: 300px; /* Adjust width as needed */\n"
         << "              margin: auto; /* Center the login form */\n"
         << "          }\n"
         << "          \n"
         << "          footer {\n"
         << "              background-color: #333;\n"
         << "              color: #fff;\n"
         << "              padding: 10px 0;\n"
         << "              text-align: center;\n"
         << "              position: fixed;\n"
         << "              bottom: 0;\n"
         << "              width: 100%;\n"
         << "          }\n"
         << "\n"
         << "          /* Style for the login form */\n"
         << "          form {\n"
         << "              display: flex;\n"
         << "              flex-direction: column;\n"
         << "          }\n"
         << "\n"
         << "          label {\n"
         << "              margin-bottom: 10px;\n"
         << "          }\n"
         << "\n"
         << "          input[type=\"text\"],\n"
         << "          input[type=\"password\"],\n"
         << "          input[type=\"submit\"] {\n"
         << "              padding: 10px;\n"
         << "              margin-bottom: 15px;\n"
         << "              border: 1px solid #ccc;\n"
         << "              border-radius: 5px;\n"
         << "              font-size: 16px;\n"
         << "          }\n"
         << "\n"
         << "          input[type=\"submit\"] {\n"
         << "              background-color: #333;\n"
         << "              color: #fff;\n"
         << "              cursor: pointer;\n"
         << "          }\n"
         << "\n"
         << "          input[type=\"submit\"]:hover {\n"
         << "              background-color: #555;\n"
         << "          }\n"
         << "      </style>\n"
         << "  </head>\n"
         << "  <body>\n"
         << "\n"
         << "  <header>\n"
         << "      <h1>.." << location << "</h1>\n"
         << "  </header>\n"
         << "  \n";
    // Open the directory
    DIR *dir;
    struct dirent *entry;
    int fileCounter = 0;
    if ((dir = opendir(location.c_str())) != NULL) {
        // Read directory entries
        while ((entry = readdir(dir)) != NULL) {
            // Skip '.' and '..'
            if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
                continue;

            // Write the file entry to the HTML
            file << "<li><a href=\"" << location.substr(14, location.length() - 1) + entry->d_name << "\">" << entry->d_name << "</a></li>\n";
            fileCounter++;
        }
        // Close the directory
        closedir(dir);
    } else {
        // Error handling if unable to open directory
        std::cerr << "Error: Unable to open directory " << location << std::endl;
        return -1;
    }

    // Write the HTML footer
    file << "</ul>\n"
         << "</section>\n"
         << "</body>\n"
         << "</html>\n";

    return fileCounter;
}