;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Name: pond-ev.el
;;; Version: 1.0©2015
;;; Author: Xiao,Hu 
;;; Intro: pond-ev mode for emacs to corporate with Pond m files.

;;; These variables you can change

(defvar pond-ev-always-start-kernel-with-mode nil
  "If t, a Pond kernel will be started every time you enter
Pond Mode (either by M-x pond-ev-mode RET or by visiting a
.ev file)."
  )

;; added by p.weitershausen@physik.tu-dresden.de
(defvar pond-ev-never-start-kernel-with-mode t
  "If t, a Pond kernel will never be started when you enter
Pond Mode (either by M-x pond-ev-mode RET or by visiting a
.ev file)."
  )
;; end addition by p.weitershausen@physik.tu-dresden.de

;;(defvar pond-ev-split-on-startup t
(defvar pond-ev-split-on-startup nil
  "If t, entering Pond mode will split the screen to show you
the kernel starting up."
  )

;;; The rest of these variables are internal

(defvar pond-kernel-workbuf ()
  "An association list connecting Pond processes with working
buffers."
  )

(defvar pond-kernel-marks ()
  "An association list connecting Pond processes with the mark
queue."
  )

(defvar pond-ev-mode-map ()
  "Keymap used in Pond-EV mode."
  )
(if pond-ev-mode-map
    ()
  (setq pond-ev-mode-map (make-sparse-keymap))
  (define-key pond-ev-mode-map "\C-j" 'pond-ev-execute)
  (define-key pond-ev-mode-map (kbd "<f5>") 'pond-ev-execute-all)
  (define-key pond-ev-mode-map "\C-ca" 'pond-abort-calculation)
  (define-key pond-ev-mode-map "\C-cs" 'pond-start-split-screen)
  (define-key pond-ev-mode-map "\C-cr" 'pond-restart-kernel)
  (define-key pond-ev-mode-map "\C-ck" 'pond-kill-this-kernel)
  (define-key pond-ev-mode-map "\C-d" 'pond-kill-this-kernel)
  (define-key pond-ev-mode-map "\177" 'backward-delete-char-untabify)
  (define-key pond-ev-mode-map ")" 'pond-evlectric-paren)
  (define-key pond-ev-mode-map "]" 'pond-evlectric-braket)
  (define-key pond-ev-mode-map "}" 'pond-evlectric-brace) 
 )

(defun pond-ev-mode ()
  "Major mode for editing Pond plain text files (.ev) in Emacs.
Commands:
\\{pond-ev-mode-map}"
  (interactive)
  (kill-all-local-variables)

  (use-local-map pond-ev-mode-map)

  (setq major-mode 'pond-ev-mode)
  (setq mode-name "Pond-EV")

  (setq local-abbrev-table pond-evode-abbrev-table)
  (set-syntax-table pond-evode-syntax-table)

  (make-local-variable 'indent-line-function)
  (setq indent-line-function 'pond-indent-line)

  (make-local-variable 'comment-start)
  (setq comment-start "(*")
  (make-local-variable 'comment-end)
  (setq comment-end "*)")
  (make-local-variable 'comment-start-skip)
  (setq comment-start-skip "\\(//+\\|/\\*+\\)\\s *\\#")

  (make-local-variable 'paragraph-start)
  (setq paragraph-start (concat page-delimiter "\\|$"))
  (make-local-variable 'paragraph-separate)
  (setq paragraph-separate paragraph-start)

  (make-local-variable 'font-lock-defaults)
  (setq font-lock-defaults '(pond-font-lock-keywords nil t))

  (make-local-variable 'kill-buffer-hook)

  ;; (if (and
  ;;       (not pond-ev-never-start-kernel-with-mode)
  ;;       (or pond-ev-always-start-kernel-with-mode
  ;;           (y-or-n-p "Start a Pond kernel for evaluations? "))
  ;;       ) ; end of logical expression
  ;;   (progn
  ;;     (pond-internal-start-kernel (current-buffer))
  ;;     (if pond-ev-split-on-startup
  ;;         (if (not (get-buffer-window "*Pond-Kernel*"))
  ;;             (pond-start-split-screen)
  ;;           ) ; end if the buffer is not visible
  ;;       ) ; end if split on startup is true
  ;;     ) ; end of progn
  ;;   (message "You can start a Pond kernel with C-c r or M-x pond-restart-kernel.")
  ;;   ) ; end if

  (run-mode-hooks 'pond-ev-mode-hook)
  )

(defun pond-ev ()
  "Start a Pond process in a new buffer."
  (interactive)
  (let ((oldval pond-ev-always-start-kernel-with-mode))
    (setq pond-ev-always-start-kernel-with-mode t)
    (switch-to-buffer (generate-new-buffer "tmp.ev"))
    (pond-ev-mode)
    (setq pond-ev-always-start-kernel-with-mode oldval)
    ) ; end let
  )

(defun pond-ev-execute (arg)
  "Executes a line of code with the Pond kernel.
   If an argument is passed, excute expression form
   current point" 
  (interactive "P")
  (if (null arg) (setq arg t) (setq arg nil)) 
  (let ((kernel (pond-kernel-from-workbuf (current-buffer))) )
    (if (processp kernel)
        (let ((start) (end) (input))
          (undo-boundary)
          ;; find a command line
          ;; start from the beginning of the line
          ;; if a argument if passed, start from current point
          ;; and ingnore whether if it is connected with previous line
          (if arg (beginning-of-line))
          (setq start (point))
          ;; move point to end of the line, coninue if encounter \ + = ... [
          (setq end (pond-find-expression-end))
          (if (> (- end start) 0) 
              (progn
                ;; get the input
                (setq input (buffer-substring start end))
                (pond-evxecute-expression input t)
                (goto-char (+ end 1)) (skip-chars-forward " \t\n")
                ) ; end of progn
            (progn (forward-char) (skip-chars-forward " \t\n"))
            ) ; end of if input is empty
          ) ; end let
      (error "No Pond kernel exist!")
      ) ; end of if
    ) ; end of let
  )

(defun pond-ev-execute-all (arg)
  "Executes the whole .ev file"
  (interactive "P")
  (let ((kernel (pond-kernel-from-workbuf (current-buffer))) )
    (if (processp kernel)
        (let ((expr) )
          (save-buffer) 
          (if (not (buffer-file-name))
              (error "please save the buffer first."))
          (setq expr (concat "Get[\"" (buffer-file-name) "\"]"))
          (pond-evxecute-expression expr t)
          ) ; end let
      (error "This buffer has no Pond kernel!")
      ) ; end of if
    ) ; end of let
  )

(provide 'pond-ev)
