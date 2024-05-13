# Index

1. [Server and Break Syntax](#server-and-break-syntax)
2. [Keywords](#keywords)
   1. [Server Syntax](#server-syntax)
      1. [Listen (Mandatory)](#listen-mandatory)
      2. [Server Name (Permissive)](#server-name-permissive)
      3. [Root (Permissive)](#root-permissive)
      4. [Index (Permissive)](#index-permissive)
      5. [Allow Methods (Mandatory)](#allow-methods-mandatory)
      6. [Error Pages (Permissive)](#error-pages-permissive)
      7. [Maximum Client Body Size (Permissive)](#maximum-client-body-size-permissive)
      8. [Locations](#locations)
         1.  [Root / Redirect (Permissive)](#root--redirect-permissive)
         2.  [Autoindex (Permissive)](#autoindex-permissive)
         3.  [CGI (Mandatory if applicable)](#cgi-mandatory-if-applicable)

# Configuration file syntax

In order to more easily define workload and objectives for each section of the project, we decided to implement this configuration syntax explanation.
As the subject for the project recommends to "get inspiration" from NGINX configuration files `nginx.conf`, we applied the following syntax regulations:

## Server and Break Syntax

A configuration file may lay out multiple servers. Each server is defined by what we call a `Server Block` (SB).
If provided with multiple SB, the program will default the first SB as the default one in case no ports match the request or there are multiple SB with the same port. For the program to function properly, **mandatory keywords** must be present with a valid value. **Permissive keywords** can be included as well. Every keyword in the configuration file should follow this syntax:

```nginx
server {
    listen 127.232.12.1:8000 ;
    server_name example ;
    root ./var/www/html ;
    index index.htm index.html index.php ;
    allow_methods GET ;

    error_page 404 /404.html ;
    error_page 500 502 503 504 /50x.html ;

    client_max_body_size 10 ;

    location {
        root ./dir/subdir ;
        allow_methods GET POST DELETE ;
    }

    location {
        redirect your_url ;
    }

    location *.extension {
        allow_methods POST ;
        cgi_pass test/tester ;
    }
}
```
Any opening or closing brackets ( `{}` ) followed by a semicolon will result in an undefined behavior error. Any content/value at the end of each parameter needs to be followed by a space and a semicolon ( ` ;` ). If any of these rules aren't followed, we will terminate the program.

**Mandatory keywords must be defined. Terminating the program otherwise.** <br>
**Permissive keywords can be omitted, defaulting to a given value.**

## Keywords

### Server Syntax

In order to maintain readability, coherence and behavior, we are defining an order to which keywords must be presented. In the original NGINX configuration we can switch up the order of things, but the downside is that it may result in undefined behavior. To prevent that, we're restricting the mutable order of keywords. The following shows the order and syntax of each keyword.

#### Listen (Mandatory)

`listen` will indicate the host and port for the server requests. You can define listen in two ways:

1. Define host and port: `listen HOST:PORT ;`
2. Define only port: `listen PORT ;`. This will automatically set any address as being able to access the server.

*NOTE: Ports under 1024 need the program to be run as sudo. This is due to the fact that these ports are normally privileged on Unix-like operating systems, including Linux. If you attempt to bind them without root privileges, you will most Define a HTTP redirection.likely encounter a binding error due to security reasons. Because of this, it is not recommended to run servers with root privileges unless it is necessary for specific functionalities that you may require. Consider using ports above 1024 for your servers.*

#### Server Name (Permissive)

`server_name` is used to define the domain of the server. If you do not define it, or define "_" as its value, any domain using the correct port can connect to the server. If more than one value is provided, the program will terminate.

    server_name VALUE ;

#### Root (Permissive)

`root` will assign the physical directory in which your web pages will be fetched/stored. If you don't define root, we will provide a default directory and web file. Root can only have a single value; otherwise, the program will terminate.

    root VALUE ;

#### Index (Permissive)

`index` defines the index/main page of the current server. This file will be fetched/stored in `root`. Not setting index will default its value to `index index.htm index.html index.php ;`. You can specify multiple values for this parameter.

    index VALUES ;

#### Allow Methods (Mandatory)

`allow_methods` will define the available actions that the client can perform in each server/location. We will only handle `GET`, `POST`, and `DELETE` methods. Any other method will cause an error. We will not set a default method for any server/location to maintain coherence with any provided page. This means if you try to access a backend admin page as a normal user and didn't set the page with any methods, we will not allow you to access the page. This not only improves security, but we believe it to be a good practice.

    allow_methods VALUES ;

#### Error Pages (Permissive)

`error_page` will set up the files for each error returned by the server response. You can set up multiple errors for each file.

    error_page ERROR_CODE ERROR_PAGE ;
    error_page ERROR_CODE ERROR_CODE ERROR_PAGE ;

*NOTE: You must create an <errors> subfolder within your root and each error family will have it's own subdirectory. This means if you setup your server's root as <./root_folder> , you must have <./root_folder/errors>. And error pages between 400 - 499 will have to be moved to <./root_folder/errors/40x>. This is a valid filepath: <./var/www/html/errors/50x/513.html>*

#### Maximum Client Body Size (Permissive)

`client_max_body_size` is mainly used to limit the amount of data a client can send via a `POST` request. Even so, it is a good practice to define this parameter. This is in order protect the server from unexpected or malicious request that attempt to overwhelm the server with large payloads. We will only accept megabytes and up to 1024MB. If not set, we will restrict the size to 128MB.

    client_max_body_size 1-1024 ;

#### Locations

From here on out you can add multiple location parameters. Each location can have either a root or a redirect. If one location provides both, the program will terminate. Each location follows the same order of keywords, but only accepts the following ones:

```nginx
location /directory {
    root ./url/page ; -- or -- redirect your_url ;
    alias ./different_dir ;
    index index.php ;
    autoindex ;
    allow_methods GET POST DELETE ;

    location *.extension {
        allow_methods GET POST DELETE ;
        cgi_pass test/tester ;
    }
}
```
##### Root / Redirect (Permissive)

`root` and `redirect` will be used to set the path for the location URL. For example, if the main root of our server is `./var/www/html` and our domain accepts any address, then when we try to access `localhost/directory`, our path will become `./var/www/html/url/page`. If redirect is found instead we will forward the request to that URL instead. If `root` nor `redirect` are found, we will set a default `redirect` page instead. You cannot define both inside the same location block. An error will be generated otherwise.

###### Redirect Guidelines

`redirect` must follow some guidelines to function properly. As the name implies, it will redirect the directory to another page or domain. To handle this behaviour, after redirecting, we close the appropriate connection. This is in case of implementing a redirection to a domain that is not within our control. That allows that domain's server to handle the connection's lifespan. When generating a redirection HTTP response there are rules that we must follow, so we can build the response correctly. Those are: redirections to other domains, must be preceded by `http://` or `https://` ; redirections to the current local server must be syntaxed like `<domain name>:<port>`, if you wish to include routing you may add it at the end, preceded by a `/`, for example, `<domain name>:<port>/<route>`.

##### Alias (Permissive)

`alias` , in our project, serves a purpose similar to `root`, but takes precedence over it. By defining an alias, when trying to access a specific file within the server, the file will first be searched within the path set as `alias`. If it's not found, only then will we search the path defined in `root`.


##### Autoindex (Permissive)

`autoindex` defines if users are allowed to access a URL via directory listing. This means, using the above example, if you try to access `localhost/directory` when it's directory listing is off, then NGINX would return a 403 Forbidden Error. Still, if a user knows the exact location path and file, they could still access it's content if they try to access `localhost/directory/index.php`. By default we will define this as `off`, for security reasons. You can set it as `on` simply by defining the keyword, no need for yes or no options.

##### CGI (Mandatory if applicable)

`cgi_pass` is used to specify the FastCGI server to which NGINX should forward requests for processing CGI scripts. When NGINX receives a request for one of these, it forwards the request to the specified FastCGI server for execution. The server then processes the script and returns the result back to NGINX, which in turn sends it back to the client. If you wish to set up behavior for scripting, then this parameter is mandatory, otherwise, the program will terminate.

    `cgi_pass VALUE` ;
