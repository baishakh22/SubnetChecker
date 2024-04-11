// Author: Muhfat Alam
// Date: April 06, 2024
// Description: Checking your IP is in the same subnet range using CIDR notation

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


// splits the input string into substrings based on the delimiter and returns a vector containing these substrings
vector<string> split(const string &s, char delim) {
  vector<string> tokens;
  string token;
  stringstream ss(s);
  while (getline(ss, token, delim)) {
    tokens.push_back(token);
  }
  return tokens;
}


// Function to check if IP in the same subnet
bool checkIP(const string &ip, const string &subnet) {
  vector<string> subnetParts = split(subnet, '/');
  if (subnetParts.size() != 2) {
    cout << "Invalid subnet format" << endl;
    return false;
  }

  string subnetAddress = subnetParts[0];
  int subnetMask;
  try {
    subnetMask = stoi(subnetParts[1]);
    if (subnetMask < 0 || subnetMask > 32) {
      cout << "Invalid subnet mask value" << endl;
      return false;
    }
  } catch (const invalid_argument &ia) {
    cout << "Invalid subnet mask value: " << ia.what() << endl;
    return false;
  }

  vector<string> ipPart = split(ip, '.');

  if (ipPart.size() != 4) {
    cout << "Invalid IP address" << endl;
    return false;
  }

  // Calculate the number of bytes that are covered by the subnet mask
  int numBytes = subnetMask / 8;
  int remainingBits = subnetMask % 8;

  // Parse subnet bytes
  vector<int> subnetBytes;
  istringstream iss(subnetAddress);
  string byte;
  while (getline(iss, byte, '.')) {
    try {
      int subnetByte = stoi(byte);
      subnetBytes.push_back(subnetByte);
    } catch (const invalid_argument &ia) {
      cout << "Invalid subnet byte value: " << ia.what() << endl;
      return false;
    }
  }

  // Handle the case of remaining bits
  if (remainingBits > 0) {
    int lastByte = subnetBytes[numBytes];      // Extract the last byte
    lastByte &= (0xFF << (8 - remainingBits)); // Mask the remaining bits
    subnetBytes[numBytes] =
        lastByte; // Update the last byte in the subnetBytes vector
  }

  for (int i = 0; i < numBytes; i++) {
    int subnetByte = subnetBytes[i];
    int ipByte = stoi(ipPart[i]);

    // Perform bitwise AND operation with Subnet Mask
    if (subnetByte != ipByte) {
      return false;
    }
  }

  // Handle the case of remaining bits
  if (remainingBits > 0) {
    int lastSubnetByte = subnetBytes[numBytes]; // Extract the last byte
    int lastIpByte = stoi(ipPart[numBytes]);

    // Perform bitwise AND operation with Subnet Mask for the remaining bits
    int subnetResult = lastSubnetByte & (0xFF << (8 - remainingBits));
    int ipResult = lastIpByte & (0xFF << (8 - remainingBits));

    if (subnetResult != ipResult) {
      return false;
    }
  }

  return true;
}

int main() {
  int response = 0;
  string subnetResponse;
  string ipResponse;

  cout << "What do need to check?";
  cout << "\n1. Verify the subnet mask: ";
  // cout << "\n2. Range of IP address: ";
  cout << "\n\n Response: ";
  cin >> response;

  if (response == 1) {
    cout << "Enter the IP address: ";
    cin >> ipResponse;
    cout << "Enter the subnet mask with CIDR notation: ";
    cin >> subnetResponse;

    if (checkIP(ipResponse, subnetResponse)) {
      cout << "\n\nIP is in the subnet range" << endl;
    } else {
      cout << "\n\nIP is not in the subnet range" << endl;
    }
  }
  return 0;
}
