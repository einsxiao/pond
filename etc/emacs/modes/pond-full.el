;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Name: pond.el
;;; Version: 1.0©2015
;;; Author: Xiao,Hu 
;;; Intro: pond mode for emacs to corporate with platform Pond.
;;;		   It is derived from from the mode written for mathematica.
;;;
;;;     pond-command-line                    "math"
;;;     pond-always-start-kernel-with-mode   nil
;;;     pond-split-on-startup                nil
(defvar pond-command-line "pond" "How to access the command-line interface to Pond on your system.") 
(defvar pond-kernel-input-max-amount 100 "Max amount of input will be recorded")
;;; The rest of these variables are internal
;;(defvar pond-startup-with-kernel-if-no-args t)
(defvar pond-startup-with-kernel-if-no-args nil)
(defvar math 'pond-start-kernel)

(setq auto-mode-alist
      (append '(("\\.ev\\'" . pond-ev-mode))   
              auto-mode-alist))

;;;(require 'pond-kernel)
(require 'pond)
(provide 'pond-full)


