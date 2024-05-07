#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
	std::unique_ptr<TODO> handler = nullptr;

	if (2 > argc)
	{
		std::cout << "The GUI is not yet implemented" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		handler = std::make_unique<TODO>(argc, argv);
		handler->run();
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}

	std::cout << "Usage: ./program file_path --replay_speed=-1 (default)/ milliseconds --mode=view (default)/ replay --page_size=10 (default)/ lines --sort=severity/timestamp";

	// open file
	// get mode: replay/ view
	// parse file
	// sort by
	// speed
	// gui

	return EXIT_SUCCESS;
}
