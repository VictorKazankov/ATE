# Go to the source root directory
cd "$(dirname "$0")/../.."

# Create a root directory for Tesseract sources and build files
mkdir -pv ../third_party/Tesseract
cd ../third_party/Tesseract

# Download trained data if necessary
readonly TESSERACT_TRAINED_DATA_ENG_URL=https://github.com/tesseract-ocr/tessdata/raw/4.00/eng.traineddata
readonly TESSERACT_TRAINED_DATA_ENG_FILENAME=`basename "$TESSERACT_TRAINED_DATA_ENG_URL"`
wget -nc "$TESSERACT_TRAINED_DATA_ENG_URL"

# Install trained data
readonly TESSERACT_TRAINED_DATA_INSTALL_DIR=/opt/Tesseract/traineddata
sudo mkdir -pv "$TESSERACT_TRAINED_DATA_INSTALL_DIR"
sudo cp "$TESSERACT_TRAINED_DATA_ENG_FILENAME" "$TESSERACT_TRAINED_DATA_INSTALL_DIR"