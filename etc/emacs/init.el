
(custom-set-variables
 '(blink-cursor-mode nil)
 '(display-time-mode t)
 '(font-use-system-font t)
 '(transient-mark-mode (quote (only . t))))
(custom-set-faces
 '(default ((t (:family "DejaVu Sans Mono" :foundry "unknown" :slant normal :weight normal :height 99 :width normal)))))

(add-to-list 'default-frame-alist '(font . "-outline-Consolas-normal-normal-normal-mono-*-*-*-*-c-*-iso10646-1" ))
(set-face-attribute 'default t :font "-outline-Consolas-normal-normal-normal-mono-*-*-*-*-c-*-iso10646-1" )

(setq make-backup-files nil)
(setq backup-by-copying t)
(setq create-lockfiles nil)

;;;;;;;;;;;;;;;;;
;;;inital size
;;(setq default-frame-alist '((height . 35) (width . 135) ) )

;;;;;;;;;;;;;;;;;
;; turn welcome screen off
(require 'package)
(let* ((no-ssl (and (memq system-type '(windows-nt ms-dos))
                    (not (gnutls-available-p))))
       (proto (if no-ssl "http" "https")))
  (when no-ssl
    (warn "\
Your version of Emacs does not support SSL connections,
which is unsafe because it allows man-in-the-middle attacks.
There are two things you can do about this warning:
1. Install an Emacs version that does support SSL and be safe.
2. Remove this warning from your init file so you won't see it again."))
  ;; Comment/uncomment these two lines to enable/disable MELPA and MELPA Stable as desired
  (add-to-list 'package-archives (cons "melpa" (concat proto "://melpa.org/packages/")) t)
  ;;(add-to-list 'package-archives (cons "melpa-stable" (concat proto "://stable.melpa.org/packages/")) t)
  (when (< emacs-major-version 24)
    ;; For important compatibility libraries like cl-lib
    (add-to-list 'package-archives (cons "gnu" (concat proto "://elpa.gnu.org/packages/")))))
(package-initialize)
(add-to-list 'package-archives
             '("melpa-stable" . "https://stable.melpa.org/packages/") t)
(add-hook 'after-init-hook 'close-welcome-buffer)
(defun close-welcome-buffer () "Close welcome buffer." (interactive) 
  (setq inhibit-startup-screen t)
  (if (bufferp "*GNU Emacs*")
      (progn 
        (kill-buffer "*GNU Emacs*")
        (message "buffer *GNU Emacs* is killed")
        ); end of progn
    (message "no buffer named *GNU Emacs* exist yet")
    ); end of if
 )

(setq-default indent-tabs-mode nil)
(show-paren-mode t)
(blink-cursor-mode (- (*) (*) (*)))
(setq linum-format "%3d  ")  ;set format  
(global-linum-mode 1) ; always show line number
;;(menu-bar-mode 1) ; hide menu bar
(setq scroll-margin 3 scroll-conservatively 10000 )
;;(set-scroll-bar-mode nil)
(fset 'yes-or-no-p 'y-or-n-p) 
(setq backup-directory-alist (quote (("." . "~/.backups"))))
(setq kill-ring-max 200)
(setq require-final-newline t)
(mouse-avoidance-mode 'animate) 
(setq default-tab-width 2)
(setq tab-width 2)
(setq split-height-threshold 0)
(setq split-width-threshold nil)
(desktop-save-mode 0)
;;----------------------------------------------------------------------
(setq etc-dir (concat (getenv "POND_ROOT") "/etc/emacs/") )
;;;--------------------------------------------------------------
;; to vim habit
(add-to-list 'load-path (concat etc-dir "evil") ) 
(require 'evil)
(evil-mode t)
(setq frame-title-format "evil-emacs@%b")
;; remember place
(require 'saveplace)
(setq-default save-place t) 
(setq save-place-file (concat etc-dir "evil-save") ) 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; auto complete
(add-to-list 'load-path (concat etc-dir "auto-complete") )
(require 'auto-complete-config)
(add-to-list 'ac-dictionary-directories (concat etc-dir "auto-complete/ac-dict") )
(ac-config-default)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(add-to-list 'load-path (concat etc-dir "modes") ) 
(require 'keybinding)


(require 'sql)
(require 'web-mode)
(require 'cuda)
(require 'pond-full)


(require 'rjsx-mode)
(add-to-list 'auto-mode-alist '("\\.js\\'" . rjsx-mode))


;;(add-to-list 'ac-modes 'web-mode)
;;(add-to-list 'ac-modes 'rjsx-mode)
;; (add-to-list 'ac-modes 'pond-kernel-mode)
;;(add-to-list 'ac-modes 'pond-ev-mode)
;; (add-to-list 'ac-modes 'pond-multi-mode)

(defun my-web-mode-hook ()
   "Hooks for Web mode."
   (setq web-mode-markup-indent-offset 2)
   (setq web-mode-css-indent-offset 2)
   (setq web-mode-code-indent-offset 2)
   (setq web-mode-attr-indent-offset 2)
   (setq web-mode-sql-indent-offset 0)
   (setq web-mode-attr-indent-offset 0)
   (setq web-mode-script-padding 0)
 )
(add-hook 'web-mode-hook  'my-web-mode-hook)


(global-set-key (kbd "RET") 'newline-and-indent)

