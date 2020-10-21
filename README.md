# CE229103 SPI

This code example includes two applications that demonstrate the operation of Serial Peripheral Interface (SPI) using the CYW20719B2 Bluetooth SoC and ModusToolbox™. 

This code example has two applications:

1. **SPI Master**: This application is used for collecting the sensor data.
2. **SPI Slave**: This application provides sensor data to the first application, i.e. SPI Master.


# Requirements
- [ModusToolbox® software](https://www.cypress.com/products/modustoolbox-software-environment) v2.2  

    **Note:** This code example version requires ModusToolbox software version 2.2 or later and is not backward compatible with v2.1 or older versions. If you cannot move to ModusToolbox v2.2, use the latest compatible version of this example: [latest-v1.X](https://github.com/cypresssemiconductorco/mtb-example-btsdk-cyw20719b2-spi/tree/latest-v1.X).  
- Board Support Package (BSP) minimum required version: 2.8.0  
- Programming Language: C
- Associated parts: [CYW20719B2](https://www.cypress.com/documentation/datasheets/cyw20719-enhanced-low-power-bredrble-bluetooth-50-soc)

## Supported Toolchains (make variable 'TOOLCHAIN')

- GNU Arm® Embedded Compiler v9.3.1 (GCC_ARM) - Default value of `TOOLCHAIN`

## Supported Kits (make variable 'TARGET')

- [CYW920719B2Q40EVB-01 Evaluation kit](https://community.cypress.com/docs/DOC-17736)

# Hardware Setup
These applications run on two separate kits. Both of them use the kit's default configuration. [Figure 1](#figure-1-block-diagram) shows the block diagram depicting the connections between different blocks of two evaluation boards.

Figure 1. Block diagram

![Block diagram](./images/block_diagram.png)

Table 1. Hardware Connections

| Function | Master GPIO    | EVB Header pin      | Arduino Header     | Slave GPIO     | EVB Header pin     | Arduino Header     |
| -------- | --------- | ----- | ---- | --------- | ----- | ---- |
| CLK      | WICED_P38 | J3.5  | D13  | WICED_P38 | J3.5  | D13  |
| MISO     | WICED_P01 | J3.6  | D12  | WICED_P01 | J3.6  | D12  |
| MOSI     | WICED_P04 | J4.1 | D7   | WICED_P04 | J4.1 | D7   |
| CS       | WICED_P07 | J3.8  | D10  | WICED_P07| J3.8  | D10  |
| GND      | GND       | J3.4  | GND  | GND       | J3.4  | GND  |


# Software Setup
Install a terminal emulator if you don't have one. Instructions in this document uses [TeraTerm](https://ttssh2.osdn.jp/index.html.en)

This example requires no additional software or tools.

# Using the Code Example

## In Eclipse IDE for ModusToolbox:

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox Application**). This launches the [Project Creator](http://www.cypress.com/ModusToolboxProjectCreator) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit and deselect the other BSPs. Keep only the required BSP in your application. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the Quick Panel. 

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.

4. Optionally, change the suggested **New Application Name**.

5. Enter the local path in the **Application(s) Root Path** field to indicate where the application needs to be created. 

   Applications that can share libraries can be placed in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox User Guide](https://www.cypress.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

    **Note**: Both the SPI Master and SPI Slave applications are created for the same kit that you have selected in Step 2.

## In Command-line Interface (CLI):

ModusToolbox provides the Project Creator as both a GUI tool and a command line tool to easily create one or more ModusToolbox applications. See the "Project Creator Tools" section of the [ModusToolbox User Guide](https://www.cypress.com/ModusToolboxUserGuide) for more details.

Alternatively, you can manually create the application using the following steps.

1. Download and unzip this repository onto your local machine, or clone the repository.

2. Open a CLI terminal and navigate to the application folder(SPI_Master or SPI_Slave).

   On Linux and macOS, you can use any terminal application. On Windows, open the **modus-shell** app from the Start menu.

    **Note:** The cloned application contains a default BSP file (*TARGET_xxx.mtb*) in the *deps* folder. Use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) (`make modlibs` command) to select and download a different BSP file, if required and deselect the other BSPs. Keep only the required BSP in your application. If the selected kit does not have the required resources or is not [supported](#supported-kits-make-variable-target), the application may not work. 

3. Import the required libraries by executing the `make getlibs` command.

Various CLI tools include a `-h` option that prints help information to the terminal screen about that tool. For more details, see the [ModusToolbox User Guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox install directory}/docs_{version}/mtb_user_guide.pdf*).

## In Third-party IDEs:

1. Follow the instructions from the [CLI](#in-command-line-interface-cli) section to create the application, and import the libraries using the `make getlibs` command.

2. Export the application to a supported IDE using the `make <ide>` command. 

    For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox User Guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox install directory}/docs_{version}/mtb_user_guide.pdf*.

3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.


# Operation

##	Using Two BT SoC Boards:

1. Connect one of the two boards to your PC using the provided USB cable through the USB connector.

2. Open a terminal program and select the WICED PUART COM port. Set the serial port parameters to 8N1 and 115200 baud. 

3. Program the board with the *SPI_master* application.

    - **Using Eclipse IDE for ModusToolbox:**

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3)**.

   - **Using CLI:**

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. You can specify a target and toolchain manually:
      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TARGET=CYW920719B2Q40EVB-01 TOOLCHAIN=GCC_ARM
      ```
4. After programming is successful, unplug first board and connect the second board to your PC. Follow the same procedure as mentioned above for *SPI_Slave* application.

   **Note**: If the download fails, it is possible that a previously loaded application is preventing programming. For example, application might use a custom baud rate that the download process does not detect or it might be in a low power mode. In that case, it may be necessary to put the board in recovery mode, and then try the programming operation again from the IDE. To enter recovery mode, first, press and hold the Recover button (SW1), then press the Reset button (SW2), release the Reset button (SW2), and then release the Recover button (SW1).

5. After programming, applications start automatically.

6. The master and slave serial terminal window display the received SPI command and the accompanying response on the terminal window, as shown in [Figure 2](#serial-terminal-output-of-spi-master) and [Figure 3](#serial-terminal-output-of-spi-slave).

   Figure 2. Serial Terminal Output of SPI master

![Serial Terminal Output of SPI master](./images/serial_terminal_output_of_spi_master.png)

​	Figure 3. Serial Terminal Output of SPI slave

![Serial Terminal Output of SPI slave](./images/serial_terminal_output_of_spi_slave.png)


# Design and Implementation

## SPI Master

This section describes the details of the implementation of the SPI Master.

On startup, the application sets up the UART and then starts the Bluetooth stack in `application_start()`. Once the stack is started (`BTM_ENABLED_EVT`), it calls the `initialize_app()` function, which handles the remaining functionality. Note that the Bluetooth stack is running; since Bluetooth is not used in this application, it does not do anything once the stack is started. The `initialize_app()` function initializes the SPI interface, RTC, GPIO and a thread to read SPI sensor.

Following is the description of the variables used:

- `dec_temp`: Holds the decimal part of the temperature reading
- `frac_temp`: Holds the fractional part of the temperature reading

In the thread handling SPI communication with the slave (`spi_sensor_thread`), a finite state machine is used to determine the data that the master requests, as shown in [Figure 4](#Finite-state-machine-adopted-for-communicating-with-slave).

Figure 4. Finite State Machine Adopted for Communicating with Slave

![Finite State Machine Adopted for Communicating with Slave](./images/finite_state_machine_adopted_for_communicating_with_slave.png)

The finite state machine contains three states:

- `SENSOR_DETECT`
- `READ_UNIT`
- `READ_TEMPERATURE`

In each state, the slave is verified to be a known slave using a packet header before processing the data that is sent from the slave. If the master is not able to authenticate the slave, the master remains in the same state and retries. After five retries, the SPI interface is reset, and the master starts from the `SENSOR_DETECT` state.

In the `SENSOR_DETECT` state, the master requests the Manufacturer ID to verify whether the slave’s manufacturer is Cypress. If the slave responds with an unknown Manufacturer ID, the master informs the user that the slave’s identity could not be authenticated. If the slave responds with the expected Manufacturer ID, the master enters the next state, `READ_UNIT`. A flowchart illustrating the operation is shown in [Figure 5](#Flowchart-of-SENSOR_DETECT-State). 

Figure 5. Flowchart of SENSOR_DETECT State

![Flowchart of SENSOR_DETECT State](./images/flowchart_of_sensor_detect_state.png)

Unit ID to know the unit of temperature values provided by the slave. If the slave responds with an unknown Unit ID, the master informs the user that the unit of temperature is unknown and tries to obtain the unit again. If the number of retries exceeds 5, the master changes its state to `SENSOR_DETECT`. Otherwise, if the slave responds as expected, the master enters the next state, `READ_TEMPERATURE`. A flowchart illustrating the operation is shown in [Figure 6](#Flowchart-of-READ_UNIT-State).

Figure 6. Flowchart of READ_UNIT State

![Flowchart of READ_UNIT State](./images/flowchart_of_read_unit_state.png)

In the `READ_TEMPERATURE` state, the master requests the temperature from the slave. The temperature reading received comprises the decimal and fractional parts of the temperature. For instance, if the temperature reading is 24.44 °C, the slave sends 2444 as the data. The master then stores the quotient as the decimal part of temperature and remainder as the fractional part of temperature in the temperature record. A flowchart illustrating the operation is shown in [Figure 7](#Flowchart-of-READ_TEMPERATURE-state). 

Figure 7. Flowchart of READ_TEMPERATURE State

![Flowchart of READ_TEMPERATURE State](./images/flowchart_of_read_temperature_state.png)



The application level source files for *spi_master* is listed in [Table 2](#table-2-application-source-files).

##### Table 2. Application Source Files

| **File Name**  | **Comments**                                                 |
| -------------- | ------------------------------------------------------------ |
| *spi_master.c* | Contains the `application_start()` function which is the entry point for execution of the user application code after device startup  and the thread that handle SPI communication with sensor. |

## SPI Slave

This section describes the operation of the slave. As with the master, `application_start()` sets up the UART and then starts the Bluetooth stack. Once the stack is started (`BTM_ENABLED_EVT`), it initializes the ADC and then calls the `initialize_app()` function which handles the remaining functionality. Note that the Bluetooth stack is running; since Bluetooth is not used in this application, it does not do anything once the stack is started. The `initialize_app()` function sets up the SPI interface, initializes the thermistor and then waits for and responds to SPI master commands. There are three commands that the slave will respond to:

- Manufacturer ID: The slave responds with its Manufacturer ID.
- Unit ID: The slave responds with its Unit ID
- Temperature: The slave responds with a temperature reading obtained by acquiring ADC samples

The slave reads from SPI Rx buffers only when its Tx buffers are empty. If the slave is unable to empty the Tx buffers after several retries, the SPI interface is reset. A flowchart illustrating the operation of the slave is shown in [Figure 8](#figure-8-spi-slave-operation).

Figure 8. SPI Slave operation

![SPI Slave operation](./images/spi_slave_operation.png)

The application level source files for “spi_slave” are listed in [Table 3](#table-3-application-source-files).

##### Table 3. Application Source Files

| **File Name**                                | **Comments**                                                 |
| -------------------------------------------- | ------------------------------------------------------------ |
| *spi_slave.c*                     | Contains the `application_start()` function which is the entry point for execution of the user application code after device startup. |


# Resources and Settings
This section explains the ModusToolbox resources and their configuration as used in this code example. Note that all the configuration explained in this section has already been done in the code example. The ModusToolbox IDE stores the configuration settings of the application in the *design.modus* file. This file is used by the graphical configurators, which generate the configuration firmware. This firmware is stored in the application’s *GeneratedSource* folder.

* **Device Configurator** The Device Configurator is used to enable/configure the peripherals and the pins used in the application. See the
[Device Configurator Guide](https://www.cypress.com/ModusToolboxDeviceConfig).

* **Bluetooth Configurator:** The Bluetooth Configurator is used for generating/modifying the BLE GATT database. See the
[Bluetooth Configurator Guide](https://www.cypress.com/ModusToolboxBLEConfig).


## Related Resources

| Application Notes                                            |                                                              |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
|**Code Examples**| 
|[Code Example for ModusToolbox](https://github.com/cypresssemiconductorco/Code-Examples-for-ModusToolbox-Software)|
|**Device Documentation**|
|[CYW20719 Device Datasheet](https://www.cypress.com/documentation/datasheets/cyw20719-enhanced-low-power-bredrble-bluetooth-50-soc)|
|**Development Kits**|
|[CYW920719B2Q40EVB-01 Evaluation kit](https://community.cypress.com/docs/DOC-17736)|
|**Tools**|
| [ModusToolbox](https://www.cypress.com/modustoolbox)     | The Cypress Development system for IoT engineers            |            |

### Other Resources

Cypress provides a wealth of data at [www.cypress.com](http://www.cypress.com) to help you to select the right device, and quickly and effectively integrate the device into your design.

## Document History

Document Title: *CE229103 – SPI*

| Version | Description of Change |
| ------- | --------------------- |
| 1.0.0   | New code example      |
| 2.0.0   | Major update to support ModusToolbox software v2.2 <br> This version is not backward compatible with ModusToolbox software v2.1  |
------

All other trademarks or registered trademarks referenced herein are the property of their respective owners.

![Banner](./images/ifx-cy-banner.png)

------

© Cypress Semiconductor Corporation, 2020. This document is the property of Cypress Semiconductor Corporation and its subsidiaries ("Cypress"). This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide. Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights. If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress's patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products. Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
 TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. No computing device can be absolutely secure. Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach"). Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach. In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes. It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product. "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage. Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices. "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness. Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, its directors, officers, employees, agents, affiliates, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress's published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
 Cypress, the Cypress logo, Spansion, the Spansion logo, and combinations thereof, WICED, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress in the United States and other countries. For a more complete list of Cypress trademarks, visit [cypress.com](https://www.cypress.com/). Other names and brands may be claimed as property of their respective owners.




