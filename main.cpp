#include <iostream>
#include <string>

// Check one entire run of digits, periods, and colons as a possible address
// Every character must fit the address-and-port format; no substring is taken
bool parseCandidate(const std::string& candidate, unsigned long& address, int& port) {
	// Keep a copy of the complete candidate for validation
	std::string token = candidate;

	// position marks the next character to read; address and port hold results
	std::size_t position = 0;
	address = 0;
	port = -1;

	// Read four octets, with one period required between each pair
	for (int octetIndex = 0; octetIndex < 4; ++octetIndex) {
		// Remember where this octet's digits begin
		std::size_t start = position;
		// Build this octet's value as its digits are read
		unsigned int octetValue = 0;

		// Consume consecutive digits and convert each character to its numeric value
		while (position < token.size() && token[position] >= '0' && token[position] <= '9') {
			// Shift the current value one decimal place, then add the new digit
			octetValue = octetValue * 10 + static_cast<unsigned int>(token[position] - '0');
			++position;
		}

		// Count how many digits were read
		std::size_t digitCount = position - start;
		// An octet must have one to three digits
		if (digitCount < 1 || digitCount > 3) {
			return false;
		}

		// Multi-digit octets cannot begin with zero, and each must fit in a byte
		if ((digitCount > 1 && token[start] == '0') || octetValue > 255) {
			return false;
		}

		// Build the 32-bit address one byte at a time
		address = address * 256 + octetValue;

		// The first three octets must each be followed by a period
		if (octetIndex < 3) {
			if (position >= token.size() || token[position] != '.') {
				return false;
			}
			++position;
		}
	}

	// If a colon is present, the candidate must also contain a valid port
	if (position < token.size() && token[position] == ':') {
		++position;
		// Save the first port digit's position
		std::size_t portStart = position;
		// Build the port value digit by digit
		unsigned int portValue = 0;

		// Read every digit after the colon and add it to the port value
		while (position < token.size() && token[position] >= '0' && token[position] <= '9') {
			portValue = portValue * 10 + static_cast<unsigned int>(token[position] - '0');
			++position;
		}

		// Count the port's digits
		std::size_t digitCount = position - portStart;
		// Reject an empty, long, zero-padded, or out-of-range port
		if (digitCount < 1 || digitCount > 5 ||
			(digitCount > 1 && token[portStart] == '0') || portValue > 65535) {
			return false;
		}
		port = static_cast<int>(portValue);
	}

	// This also rejects extra periods, colons, and any other unconsumed text
	return position == token.size();
}

bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
	// Set the required failure values before searching
	outAddress = 0;
	outPort = -1;

	// position moves through the input from left to right
	std::size_t position = 0;
	while (position < str.size()) {
		// Skip separators; only digits, periods, and colons belong to a candidate
		if (!((str[position] >= '0' && str[position] <= '9') ||
			  str[position] == '.' || str[position] == ':')) {
			++position;
			continue;
		}

		// Mark the start of a candidate, then consume its full contiguous run
		std::size_t start = position;
		while (position < str.size() &&
			   ((str[position] >= '0' && str[position] <= '9') ||
				str[position] == '.' || str[position] == ':')) {
			++position;
		}

		// Copy the complete run so it can be checked as one candidate
		std::string candidate = str.substr(start, position - start);
		unsigned long candidateAddress = 0;
		int candidatePort = -1;
		// Return the first valid candidate; otherwise continue after this run
		if (parseCandidate(candidate, candidateAddress, candidatePort)) {
			outAddress = candidateAddress;
			outPort = candidatePort;
			return true;
		}
	}

	return false;
}

int main() {
	// Store each complete line entered by the user
	std::string input;

	// Keep prompting until the user enters exactly END or input ends
	while (true) {
		std::cout << "Enter a string (or 'END' to quit):\n";
		if (!std::getline(std::cin, input)) {
			break;
		}

		if (input == "END") {
			std::cout << "Program terminated.\n";
			break;
		}

		// Receives the numeric IPv4 address
		unsigned long address = 0;
		// Receives the port, or stays -1 when none was found
		int port = -1;
		if (!extractIPv4(input, address, port)) {
			std::cout << "Invalid input: no valid IPv4 address found\n";
			continue;
		}

		// Divide by each byte's place value to recover the four octets
		unsigned long first = address / 16777216UL;
		unsigned long second = (address / 65536UL) % 256UL;
		unsigned long third = (address / 256UL) % 256UL;
		unsigned long fourth = address % 256UL;

		// Print the octets, full decimal value, and either the port or "none"
		std::cout << "Extracted IPv4 address: "
				  << first << '.' << second << '.' << third << '.' << fourth
				  << " (decimal value: " << address << ", port: ";
		if (port == -1) {
			std::cout << "none";
		} else {
			std::cout << port;
		}
		std::cout << ")\n";
	}

	return 0;
}
