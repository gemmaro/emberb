(use-modules (gnu packages gettext)
             (gnu packages node))

(packages->manifest (list po4a node-lts))
