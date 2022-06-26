#include <boost/process.hpp>
#include <boost/detail/winapi/process.hpp>
#include <iostream>

struct new_window
    : ::boost::process::detail::handler_base
{
    template <class WindowsExecutor>
    void on_setup(WindowsExecutor& e) const
    {
        e.creation_flags = ::boost::detail::winapi::CREATE_NEW_CONSOLE_;
        e.startup_info.dwFlags |= ::boost::detail::winapi::STARTF_USEPOSITION_;
        e.startup_info.dwY |= 500;
    }
};


int main()
{
	setlocale(LC_ALL, "rus");
	
	std::string lpszAppName = "Child.exe";
	std::string lpszAppName2 = "Count.exe";
	std::string lpszCommandLine;

	lpszCommandLine = lpszAppName.c_str();
	lpszCommandLine += " 10 5 4 3 7 13 19 35 22 9 12 5 34 45";

	boost::process::child c1(lpszCommandLine, new_window{});
	std::cout << "Создан новый процесс.\n";

	boost::process::child c2(lpszAppName2, new_window{});
	std::cout << "Создан новый процесс.\n";

	c1.wait();
	c2.wait();

}