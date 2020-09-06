project('libdisc', 'c')

src = ['disc.c', 'disc_app.c', 'disc_endpoint.c']
libdisc = shared_library('libdisc', src)

libdisc_dep = declare_dependency(link_with : libdisc)