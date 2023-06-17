(use-modules (gnu packages gettext)
             (gnu packages node)
             (gnu packages ruby)
             (guix packages)
             (guix download)
             (guix build-system ruby)
             ((guix licenses) #:prefix license:)
             (gnu packages perl)
             (gemmaro packages perl))

(setenv "PERL5LIB" (string-append (getenv "PWD") "/_translation/po4a/lib:" (getenv "PERL5LIB")))

(define-public ruby-guard
  (package
    (name "ruby-guard")
    (version "2.18.0")
    (source (origin
              (method url-fetch)
              (uri (rubygems-uri "guard" version))
              (sha256
               (base32
                "1zqy994fr0pf3pda0x3mmkhgnfg4hd12qp5bh1s1xm68l00viwhj"))))
    (build-system ruby-build-system)
    (arguments
     `(#:tests? #f)) ;TODO
    (propagated-inputs (list ruby-formatador
                             ruby-listen
                             ruby-lumberjack
                             ruby-nenv
                             ruby-notiffany
                             ruby-pry
                             ruby-shellany
                             ruby-thor))
    (synopsis
     "Guard is a command line tool to easily handle events on file system modifications.")
    (description
     "Guard is a command line tool to easily handle events on file system
modifications.")
    (home-page "http://guardgem.org")
    (license license:expat)))

(define-public ruby-guard-compat
  (package
    (name "ruby-guard-compat")
    (version "1.2.1")
    (source (origin
              (method url-fetch)
              (uri (rubygems-uri "guard-compat" version))
              (sha256
               (base32
                "1zj6sr1k8w59mmi27rsii0v8xyy2rnsi09nqvwpgj1q10yq1mlis"))))
    (build-system ruby-build-system)
    (arguments
     `(#:tests? #f)) ;TODO
    (synopsis "Helps creating valid Guard plugins and testing them")
    (description "Helps creating valid Guard plugins and testing them")
    (home-page "")
    (license license:expat)))

(define-public ruby-guard-shell
  (package
    (name "ruby-guard-shell")
    (version "0.7.2")
    (source (origin
              (method url-fetch)
              (uri (rubygems-uri "guard-shell" version))
              (sha256
               (base32
                "0va3brhgc0lsvayjizyhzdybikzlyx9kfqpdgdglclbwb5611kf9"))))
    (build-system ruby-build-system)
    (arguments
     `(#:tests? #f)) ;TODO
    (propagated-inputs (list ruby-guard ruby-guard-compat))
    (synopsis
     "    Guard::Shell automatically runs shell commands when watched files are
    modified.
")
    (description
     "Guard::Shell automatically runs shell commands when watched files are modified.")
    (home-page "http://github.com/sdwolfz/guard-shell")
    (license license:expat)))

(packages->manifest (list po4a node-lts jekyll ruby-guard ruby-guard-shell ruby-rubocop ruby tidyall perl-perl-tidy perl))
