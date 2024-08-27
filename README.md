Firmware Update using GitHub Repo
================================

This is an example sketch for updating the firmware of an ESP8266 board using a GitHub repository.

Features
--------

* Uses HTTPS (TLS) to connect to the GitHub repository
* Checks for firmware updates by comparing the current version with the version on the repository
* Downloads and updates the firmware if a new version is available
* Provides progress updates and error handling

Configuration
-------------

* You need to create a `config.h` file with your WiFi credentials (SSID and password) you can use `config_example.h`
* You need to create a `certs.h` file with the DigiCert Global Root G2 certificate with the help of `certUpdate.sh` for Linux, `certUpdate.bat` for Windows
* You need to update the `github_host`, `github_port`, `URL_VERSION_FILE_PATH`, and `URL_fw_Bin` variables with your GitHub repository information

How to Use
-----------

1. Create a GitHub repository with your firmware binary files
2. Update the `config.h` and `certs.h` files with your WiFi credentials and certificate
3. Update the `github_host`, `github_port`, `URL_VERSION_FILE_PATH`, and `URL_fw_Bin` variables with your GitHub repository information
4. Upload the sketch to your ESP8266 board
5. The board will connect to the WiFi network and check for firmware updates
6. If a new version is available, the board will download and update the firmware and reboot itself in 2 second

Note
----

  **This firmware based on**

* `ESP8266httpUpdate` library to perform the firmware update
* `WiFiClientSecure` library to connect to the GitHub repository using HTTPS

License
-------

This sketch is licensed under the MIT License.

Author
------

*THE EVEN*