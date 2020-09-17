# See the file "COPYING" in the main distribution directory for copyright.

# This plugin supports Endace DAG card load balancing across multiple consecutive (even) receive streams.
# Does not currently configure DAG card for steering. See README for useage instructions.
# Supports broctl.cfg PFRINGFirstAppInstance to specify starting stream number.

try:
    from ZeekControl import plugin
    from ZeekControl import config
except ImportError:
    from BroControl import plugin
    from BroControl import config

class LBDAG(plugin.Plugin):
    def __init__(self):
        super(LBDAG, self).__init__(apiversion=1)

    def name(self):
        return "lb_dag"

    def pluginVersion(self):
        return 1

    def init(self):
        useplugin = False
        first_app_instance = int(config.Config.pfringfirstappinstance)
        app_instance = first_app_instance
        host = None
        for nn in self.nodes():
            if nn.type != "worker" or not nn.interface.startswith("endace::") or not nn.lb_procs or nn.lb_method != "custom":
                continue

            # Reset stream numbers for different hosts
            if nn.host != host:
                app_instance = first_app_instance
                host = nn.host

            useplugin = True

            # For the case where a user is running dag HLB
            nn.interface = "%s:%d" % (nn.interface, app_instance*2)

            app_instance += 1

        return useplugin