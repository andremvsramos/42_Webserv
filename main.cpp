/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-sous <lde-sous@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 15:36:38 by lde-sous          #+#    #+#             */
/*   Updated: 2024/02/06 15:36:38 by lde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "headers/config/confParser.hpp"
// #include "headers/server/ServerCluster.hpp"
#include "./headers/webserv.hpp"
#include "./headers/server/Server.hpp"
#include "./headers/server/ServerCluster.hpp"

//ServerCluster* GlobalServerPtr = NULL;
volatile sig_atomic_t gSignalStatus = 0;
ServerCluster* gServerCluster = NULL;
bool chunky = false;


/**
 * @brief Signal handler function for handling signals in the program.
 *
 * @param signum The signal number.
 */
extern "C" void	signalHandler(int signum) {

    if (gServerCluster) {
        std::vector<Server*>::const_iterator it;
        for (it = gServerCluster->getServers().begin(); it != gServerCluster->getServers().end(); ++it)
            close((*it)->getFD());
       // gServerCluster->ClearServer();
    }
    (void)signum;
	gSignalStatus = 1;
    const char* loadingIcons[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    const int numIcons = sizeof(loadingIcons) / sizeof(loadingIcons[0]);

    std::cout << "Closing program safely ";
    std::cout.flush();

    for (int j = 0; j < 10; ++j) {
        std::cout << "\r" << BOLD << YELLOW << "Closing program safely... " << loadingIcons[j % numIcons] << RESET;
        std::cout.flush();
        usleep(100000); // Sleep for 100 milliseconds (adjust as needed)
    }
    std::cout << "\r" << BOLD << GREEN << "Closing program safely \u2713" << RESET;
    std::cout << "                 \n"; // Clear the rest of the line
    std::cout << std::endl;
}

static void printWebServLogo(char** envp) {
    pid_t pid = fork();
    if (pid == 0) {
        char* argv[] = {NULL};
        if (execve("/usr/bin/clear", argv, envp) == -1)
            exit (EXIT_FAILURE);
    }
    waitpid(pid, NULL, 0);
    std::cout << BOLD << CYAN << " /██      /██ /████████ /███████   /██████  /████████ /███████  /██    /██\n";
    std::cout << "| ██  /█ | ██| ██_____/| ██__  ██ /██__  ██| ██_____/| ██__  ██| ██   | ██\n";
    std::cout << "| ██ /███| ██| ██      | ██  \\ ██| ██  \\__/| ██      | ██  \\ ██| ██   | ██\n";
    std::cout << "| ██/█ ██ ██| █████   | ███████ |  ██████ | █████   | ██████/|    ██ / ██/\n";
    std::cout << "| ████_  ████| ██__/   | ██__  ██ \\____  ██| ██__/   | ██__  ██ \\  ██ ██/ \n";
    std::cout << "| ███/ \\  ███| ██      | ██  \\ ██ /██  \\ ██| ██      | ██  \\ ██  \\  ███/  \n";
    std::cout << "| ██/   \\  ██| ███████| ███████/|  ██████/| ████████ | ██  | ██   \\  ██/   \n";
    std::cout << "|__/     \\__/|________/|_______/  \\______/ |________/|__/  |__/    \\_/    \n" << RESET << std::endl;
}

int		main(int ac, char **av, char** envp)
{
    printWebServLogo(envp);
    std::string filename;
    signal(SIGPIPE, SIG_IGN);
    if (ac != 2)
        std::cerr << RED << "Run the program with a configuration file: ./webserv [configuration file]" << RESET << std::endl;
    else {
        signal(SIGINT, signalHandler);
        ServerCluster cluster(av[1]);
        gServerCluster = &cluster;
        cluster.StartServers();
    }
	return EXIT_SUCCESS;
}
