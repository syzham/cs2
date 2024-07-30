#include "memory.h"

bool driver::attach_to_process(HANDLE driver_handle, const DWORD pid) {
	driver::Request r;
	r.process_id = reinterpret_cast<HANDLE>(pid);

	return DeviceIoControl(driver_handle, codes::attach, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
}
const char* driver::read_string32(HANDLE driver_handle, const std::uintptr_t addr, int i){
	char letter;
	int start = i;
	char* string = new char[128];
	do {
		letter = driver::read_memory<char>(driver_handle, addr + sizeof(char) * i);
		string[i - start] = letter;
		i++;
	} while (letter != NULL);
	return string;
}