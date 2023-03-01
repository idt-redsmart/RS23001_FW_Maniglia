
#include <def.h>
#include <Bluetooth.h>
#include <LoadCell.h>
#include <ShockDetection.h>

LoadCell Scale;
Bluetooth BT;
ShockDetection Shock;

FireTimer batt;

void setup()
{
// put your setup code here, to run once:
#ifdef DEBUG
    Serial.begin(115200);
    while (!Serial)
    {
    }
#endif
    // delay(500);

    nicla::begin();
    nicla::enableCharge(100);

    BHY2.begin(NICLA_STANDALONE);

    Scale.setup();
    BT.setup();
    Shock.setup();

    batt.begin(5000);
    batt.start();
}

void loop()
{
    // Update function should be continuously polled
    BHY2.update();

    BT.poll();
    BT.authentication();
    BT.checkCentralConnected();
    BT.writeShockDetect();
    BT.writeWeight();

    Shock.detect();

    batt.start();
    if (batt.fire())
    {
        batt.stop();
        Serial.println("BATTERIA: " + String(nicla::getBatteryStatus()));
    }
}

/*
#include <BlockDevice.h>
#include <Dir.h>
#include <File.h>
#include <FileSystem.h>
#include <LittleFileSystem.h>

// The name of the root of the filesystem
constexpr auto userRoot { "fs" };

// The SPIF Block Device
mbed::BlockDevice* spif;
// The LittleFS filesystem
mbed::LittleFileSystem fs { userRoot };

//proto
void flash_init();
void listDirsAlt();
void storeData();
void printStats();
void printFile(mbed::File& file);
*/
/*
void flash_init()
{
  // Get core-wide instance of SPIF Block Device
    spif = mbed::BlockDevice::get_default_instance();
    spif->init();

    // Mount the filesystem
    int err = fs.mount(spif);
    if (err) {
        err = fs.reformat(spif);
        Serial.print("Error mounting file system: ");
        Serial.println(err);
        while (true)
            ;
    }
}

void listDirsAlt()
{
    String baseDirName = "/";
    baseDirName += userRoot;

    Serial.print("Listing file on ");
    Serial.print(baseDirName);
    Serial.println(" Filesystem");

    // Open the root of the filesystem
    mbed::Dir dir(&fs, "/");
    dirent ent;

    // Cycle through all the directory entries
    while ((dir.read(&ent)) > 0) {
        switch (ent.d_type) {
        case DT_DIR: {
            Serial.print("Directory ");
            Serial.println(ent.d_name);
            break;
        }
        case DT_REG: {
            Serial.print("Regular File ");
            Serial.print(ent.d_name);

            // Declare and open the file in read-only mode
            mbed::File file;
            auto ret = file.open(&fs, ent.d_name);
            if (ret) {
                Serial.println("Unable to open file");
                continue;
            }
            Serial.print(" [");
            Serial.print(file.size());
            Serial.println(" bytes]");

            if (file.size() > 0) {
                // Print file with an ad-hoc function. YMMV.
                printFile(file);

                // Empty file after reading all the content. YMMV.
                file.close();
                ret = file.open(&fs, ent.d_name, O_TRUNC);
                if (ret < 0)
                    Serial.println("Unable to truncate file");
            } else {
                // Remove file if empty. YMMV.
                file.close();
                fs.remove(ent.d_name);
            }

            break;
        }
        default: {
            Serial.print("Other ");
            break;
        }
        }
    }
}

void storeData()
{
    // Append data to file on LittleFS filesystem
    constexpr auto filename { "sensors.csv" };

    // Open in file in write mode.
    // Create if doesn't exists, otherwise open in append mode.
    mbed::File file;
    auto err = file.open(&fs, filename, O_WRONLY | O_CREAT | O_APPEND);
    if (err) {
        Serial.print("Error opening file for writing: ");
        Serial.println(err);
        return;
    }

    // Save sensors data as a CSV line
    String csvLine = "Suca";//sensorsToCSVLine();

    auto ret = file.write(csvLine.c_str(), csvLine.length());
    if (ret != csvLine.length()) {
        Serial.print("Error writing data: ");
        Serial.println(ret);
    }
    file.close();
}

void printStats()
{
    struct statvfs stats { };
    fs.statvfs(userRoot, &stats);

    auto blockSize = stats.f_bsize;

    Serial.print("Total Space [Bytes]:  ");
    Serial.println(stats.f_blocks * blockSize);
    Serial.print("Free Space [Bytes]:   ");
    Serial.println(stats.f_bfree * blockSize);
    Serial.print("Used Space [Bytes]:   ");
    Serial.println((stats.f_blocks - stats.f_bfree) * blockSize);
    Serial.println();
}

void printFile(mbed::File& file)
{
    // Read and print file len-bytes at time
    // to preserve RAM
    constexpr size_t len { 256 };

    size_t totalLen { file.size() };

    while (totalLen > 0) {
        char buf[len] {};

        auto read = file.read(buf, len);
        totalLen -= read;
        for (const auto& c : buf)
            Serial.print(c);
    }
    Serial.println();
}
*/