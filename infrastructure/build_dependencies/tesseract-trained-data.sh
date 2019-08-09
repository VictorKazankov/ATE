# Go to the source root directory
cd "$(dirname "$0")/../.."

readonly ROOT_DIR=$(pwd -P)

# Get Tesseract version
. infrastructure/build_dependencies/version.sh

# Create a root directory for Tesseract sources and build files
mkdir -pv ../third_party/Tesseract
cd ../third_party/Tesseract

# Download trained data if necessary
readonly TESSERACT_TRAINED_DATA_ENG_URL=https://github.com/tesseract-ocr/tessdata_fast/raw/$VHAT_TESSERACT_VERSION/eng.traineddata
readonly TESSERACT_TRAINED_DATA_ENG_FILENAME=`basename "$TESSERACT_TRAINED_DATA_ENG_URL"`
wget -nc "$TESSERACT_TRAINED_DATA_ENG_URL"

# Install trained data
readonly TESSERACT_TRAINED_DATA_INSTALL_DIR=/usr/local/share/tessdata/
sudo mkdir -pv "$TESSERACT_TRAINED_DATA_INSTALL_DIR"
sudo cp -v "$TESSERACT_TRAINED_DATA_ENG_FILENAME" "$TESSERACT_TRAINED_DATA_INSTALL_DIR"

# Create deb package with trained data
readonly PACKAGE_ARCH=$(dpkg-architecture -q DEB_HOST_ARCH)
readonly PACKAGE_NAME=tesseract-trained-data
readonly PACKAGE_DIR_NAME="${PACKAGE_NAME}-${VHAT_TESSERACT_VERSION}_${PACKAGE_ARCH}"
readonly PACKAGE_SIZE=$(sh -c "du $TESSERACT_TRAINED_DATA_ENG_FILENAME | sed 's/$TESSERACT_TRAINED_DATA_ENG_FILENAME//'")

# Set up tmp packaging directory
if [ -e $PACKAGE_DIR_NAME ]; then rm -rvf $PACKAGE_DIR_NAME; fi
mkdir $PACKAGE_DIR_NAME
mkdir $PACKAGE_DIR_NAME/DEBIAN
mkdir -p $PACKAGE_DIR_NAME/$TESSERACT_TRAINED_DATA_INSTALL_DIR

# Copy trained data file inside
cp -v "$TESSERACT_TRAINED_DATA_ENG_FILENAME" "$PACKAGE_DIR_NAME/$TESSERACT_TRAINED_DATA_INSTALL_DIR"

# Prepare debian control
m4 \
  -DNAME=$PACKAGE_NAME \
  -DVERSION=$VHAT_TESSERACT_VERSION \
  -DARCH=$PACKAGE_ARCH \
  -DDEPENDS= \
  -DCONFLICTS=tesseract-ocr-eng \
  -DSIZE=$PACKAGE_SIZE \
  -DDESCRIPTION="Tesseract trained data for VHAT needs" \
  "$ROOT_DIR/infrastructure/build_dependencies/debian-control.m4" > $PACKAGE_DIR_NAME/DEBIAN/control

# And pack everything
fakeroot dpkg-deb --build $PACKAGE_DIR_NAME
