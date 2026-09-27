# AI Usage Disclosure

## Generative AI Tool Used
I used GitHub Copilot Chat with Auto model selection on September 27, 2026. Because Copilot was set to Auto, I did not select or record a specific underlying model.

## How AI Was Used
I used GitHub Copilot Chat to generate the initial C++ implementation of the IPv4 extraction program and later correct an issue found during testing. I reviewed and tested the generated code rather than assuming that the initial output was correct.

## Prompt 1: Initial Code Generation
I gave GitHub Copilot Chat the following prompt:

I need help implementing a C++ program for a class assignment that extracts a valid IPv4 address, optionally followed by a port number, from a line of otherwise unstructured text.

Please implement this function exactly:

`bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort);`

Requirements:

- Scan the input string for a valid IPv4 address embedded anywhere in the text.
- Only digits, periods (.), and colons (:) can be part of a candidate token. All other characters are garbage/separators and should be skipped.
- A candidate must be validated in full. Do not extract a valid substring from inside a malformed contiguous run of digits, periods, and colons.
- An IPv4 address must contain exactly four octets separated by exactly three periods.
- Each octet must contain 1-3 digits.
- Each octet must have a value from 0 through 255.
- Leading zeros are not allowed unless the octet is exactly "0".
- The fourth octet may optionally be immediately followed by a period.
- A port must contain 1-5 digits and have a value from 0 through 65535.
- A port cannot have a leading zero unless it is exactly "0".
- If a colon is present but the port is invalid, reject the entire candidate, including the otherwise-valid IPv4 address.
- Reject malformed candidates such as extra periods, extra colons, empty octets, missing octets, or punctuation directly adjacent to an otherwise-valid address.
- If one candidate is invalid, continue scanning later portions of the input for another candidate.
- On success, `outAddress` should contain the IPv4 address as a 32-bit numeric value and `outPort` should contain the port, or -1 if no port was present.
- On failure, set `outAddress` to 0 and `outPort` to -1.
- Do not use `atoi`, `atol`, `atoll`, `strtol`, `strtoul`, `strtod`, `stoi`, `stol`, `stoul`, `sscanf`, `scanf` numeric conversions, regular expressions, `inet_aton`, `inet_pton`, `inet_addr`, or similar parsing/conversion functions. Parse characters and accumulate all numeric values manually.

Also write `main()` so that it:

- Repeatedly prompts: Enter a string (or 'END' to quit):
- Stops only when the complete input line is exactly END.
- Prints "Program terminated." before exiting.
- Calls `extractIPv4` for other input.
- On success prints: `Extracted IPv4 address: A.B.C.D (decimal value: N, port: P)` where P is the port number or "none".
- On failure prints: `Invalid input: no valid IPv4 address found`

## Prompt 2: Correct Trailing-Period Validation
After testing, I reported:

I tested the program against the assignment requirements and found a bug in the generated code.

The input `192.168.1.1.` is supposed to be invalid. The assignment requires a stray period or colon directly adjacent to an otherwise-valid IPv4 address to invalidate the entire candidate. A candidate must match the grammar in full, with no truncating or removing characters to create a valid address.

However, the current `parseCandidate` function removes one trailing period because it treats it as sentence punctuation. This causes `192.168.1.1.` to incorrectly become `192.168.1.1` and be accepted.

Fix this bug so the entire contiguous candidate run of digits, periods, and colons is always validated exactly as it appears in the input. Do not remove a trailing period or colon.

Keep the rest of the parsing behavior unchanged because the other sample and boundary tests I ran passed.

Keep the existing concise, beginner-friendly comments and explain the small change you make.

## Code Attribution and Modifications
GitHub Copilot Chat generated the initial implementation of the program, including `main()`, `extractIPv4()`, and the `parseCandidate()` helper function. I reviewed the generated code and tested it against the assignment requirements and additional test cases.

During testing, I found that the initial code incorrectly accepted `192.168.1.1.` because it removed the trailing period before validation. I identified this as conflicting with the requirement that the entire candidate must be validated. I then prompted Copilot to correct this behavior and verified the correction by testing the input again.

## Testing
I tested the program using the sample inputs from the assignment as well as additional cases stored in `tests.txt`. My tests included valid addresses, valid ports, boundary values such as octets of 0 and 255 and ports of 0 and 65535, values above those limits, leading zeros, missing or extra octets, extra periods and colons, invalid ports, and malformed candidates followed by valid candidates.

After correcting the trailing-period issue, the program produced the expected results for the test cases I ran.

## Verification Statement
I have reviewed the submitted code and understand how each part works, including how candidate tokens are found, how digits are manually converted into numeric values, how octets and ports are validated, how the 32-bit IPv4 value is calculated, and how the final output is formatted.

I have tested the submitted code and, based on my testing, it works as intended. I am not currently aware of any unresolved bugs, limitations, or unexpected behavior.
