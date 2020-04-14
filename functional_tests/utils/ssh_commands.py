
_VDP_ATE_SERVER = 'vdp_ate_server.service'
VDP_ATE_SERVER_STOP = 'sudo systemctl stop {}'.format(_VDP_ATE_SERVER)
VDP_ATE_SERVER_START = 'sudo systemctl start {}'.format(_VDP_ATE_SERVER)
VDP_ATE_SERVER_RESTART = 'sudo systemctl restart {}'.format(_VDP_ATE_SERVER)

SUDO_COPY_FILE = 'sudo cp {} {}'
COPY_FILE = 'cp {} {}'

SUDO_TOUCH = 'sudo touch {}'
SUDO_CHMOD = 'sudo chmod {} {}'

SYNC3_SH = '/usr/share/vdp-mcu-utils/sync3.sh -d {} \n'
