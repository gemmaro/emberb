(use-modules (gnu packages gcc)
             (gnu packages pkg-config)
             (gnu packages sdl)
             (gnu packages ruby)
             (gnu packages glib)
             (gnu packages base))

(setenv "CC" "gcc")

(packages->manifest (list gcc
                          pkg-config
                          sdl2
                          ruby-3.0
                          glibc
                          binutils))
