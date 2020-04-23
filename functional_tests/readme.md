# How to run functional tests on your local machine
1. Required a PC with Ubuntu 16.04 LTS or later

2. Install [tox](https://tox.readthedocs.io/en/latest/install.html).

3. Clone project ATE on your machine, build and install ATE client. Refer to project [README](../README.md) or product documentation for instructions.

4. Configure vhat_client. Edit file `/usr/local/etc/vdp/ate_client.ini` on Ubuntu or `C:\ProgramData\VDP\ate_client.ini` on Windows:
    - In a`[BOARD]` section set `Address` to ip or hostname of VDP board. e.g.:
        ```
        [BOARD]
        #This section configures LVDS board
        Address = 192.168.50.1
        Port = 5000
        ```

5.  Go to `ATE/functional_tests` folder.

6. Run `tox -- [path_to_file or directory]` command.
 
 Run `tox -- -k [test suite name] ` command.

 Example to run regression:
     ```
     tox -r -- -k regression_sync3, tox -r -- -k regression_sync4
     ```

# How to generate test report locally
1. Install Allure commandline utility by [link](https://docs.qameta.io/allure/#_manual_installation).
    
    **To run commandline application, Java Runtime Environment must be installed and allure must be added to system PATH.**

    ```
    export PATH=/path-to-alulure/allure-2.13.0/bin:$PATH
    ```
**Cautions:** Do not install via apt-get install because allure version of that repo is outdated (2.4.1). It's required later version (eg. [2.13.0](http://repo.maven.apache.org/maven2/io/qameta/allure/allure-commandline/2.13.0/)) to generated report correctly

2. Run your tests and after running it will generate _allure-results_ folder.

3. From the _functional_tests_ directory run `allure generate allure-results --clean -o allure-report` in the command prompt.

4. On successful execution, it will generate _allure-report_ folder in _functional_tests_ directory.

5. Execute `allure open allure-report` to open the report.

# How to open the report on a different machine.
The allure report should be host by a local server. Opening file index.html inside the report folder will not work.
 - For Windows: Open `allure-report/index.html` by Microsoft Edge.
 - For Linux + Windows without Microsoft Edge: Install Allure commandline utility as above.
