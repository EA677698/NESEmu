def sanitize_emulator_log(input_file, output_file):
    # Lines to keep must contain "[debug]" and not contain irrelevant information
    skip_keywords = ["PC REGISTER", "INITIAL OPCODE", "Status:", "[info]"]

    with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
        for line in infile:
            # Write line to output only if it's a [debug] line and does not contain skip keywords
            if "[debug]" in line and not any(keyword in line for keyword in skip_keywords):
                outfile.write(line)