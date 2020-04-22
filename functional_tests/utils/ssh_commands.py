ATE_SERVER_CONFIG = '/etc/vdp/ate_server.ini'
GET_CURRENT_WIDTH = r'cat {} | grep FrameWidth'.format(ATE_SERVER_CONFIG)
GET_CURRENT_HEIGHT = r'cat {} | grep FrameHeight'.format(ATE_SERVER_CONFIG)

_SUDO_SYSTEMCTL_STOP = 'sudo systemctl stop {}'
_SUDO_SYSTEMCTL_START = 'sudo systemctl start {}'
_SUDO_SYSTEMCTL_RESTART = 'sudo systemctl restart {}'

_VDP_ATE_SERVER = 'vdp_ate_server.service'
VDP_ATE_SERVER_STOP = _SUDO_SYSTEMCTL_STOP.format(_VDP_ATE_SERVER)
VDP_ATE_SERVER_START = _SUDO_SYSTEMCTL_START.format(_VDP_ATE_SERVER)
VDP_ATE_SERVER_RESTART = _SUDO_SYSTEMCTL_RESTART.format(_VDP_ATE_SERVER)

_VDP_MEDIA_SERVER = 'vdp_media_server.service'
VDP_MEDIA_SERVER_STOP = _SUDO_SYSTEMCTL_STOP.format(_VDP_MEDIA_SERVER)
VDP_MEDIA_SERVER_START = _SUDO_SYSTEMCTL_START.format(_VDP_MEDIA_SERVER)
VDP_MEDIA_SERVER_RESTART = _SUDO_SYSTEMCTL_RESTART.format(_VDP_MEDIA_SERVER)

SUDO_COPY_FILE = r'sudo cp "{}" "{}"'
COPY_FILE = r'cp "{}" "{}"'

SUDO_MKDIR = r'sudo mkdir "{}"'
SUDO_TOUCH = r'sudo touch "{}"'
SUDO_CHMOD = r'sudo chmod {} "{}"'
SUDO_RM = r'sudo rm -r "{}"'
SUDO_CHOWN = r'sudo chown {} "{}"'

SYNC3_SH = '/usr/share/vdp-mcu-utils/sync3.sh -d {} \n'

GET_AVAIL_SPACE = r'df / --output=avail'
