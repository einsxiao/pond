;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Name: pond.el
;;; Version: 1.0©2015
;;; Author: Xiao,Hu 
;;; Intro: pond mode for emacs to corporate with Pond m files.

;;; These variables you can change

(defvar pond-always-start-kernel-with-mode nil
  "If t, a Pond kernel will be started every time you enter
Pond Mode (either by M-x pond-mode RET or by visiting a
.pd file)."
  )

;; added by p.weitershausen@physik.tu-dresden.de
(defvar pond-never-start-kernel-with-mode t
  "If t, a Pond kernel will never be started when you enter
Pond Mode (either by M-x pond-mode RET or by visiting a
.pd file)."
  )
;; end addition by p.weitershausen@physik.tu-dresden.de

;;(defvar pond-split-on-startup t
(defvar pond-split-on-startup nil
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

(defvar pond-mode-map ()
  "Keymap used in Pond mode."
  )
(if pond-mode-map
    ()
  (setq pond-mode-map (make-sparse-keymap))
  (define-key pond-mode-map "\C-j" 'pond-execute)
  (define-key pond-mode-map (kbd "<f5>") 'pond-execute-all)
  (define-key pond-mode-map "\C-ca" 'pond-abort-calculation)
  (define-key pond-mode-map "\C-cs" 'pond-start-split-screen)
  (define-key pond-mode-map "\C-cr" 'pond-restart-kernel)
  (define-key pond-mode-map "\C-ck" 'pond-kill-this-kernel)
  (define-key pond-mode-map "\C-d" 'pond-kill-this-kernel)
  (define-key pond-mode-map "\177" 'backward-delete-char-untabify)
  (define-key pond-mode-map ")" 'pond-evlectric-paren)
  (define-key pond-mode-map "]" 'pond-evlectric-braket)
  (define-key pond-mode-map "}" 'pond-evlectric-brace) 
 )

(defun pond-mode ()
  "Major mode for editing Pond plain text files (.pd) in Emacs.
Commands:
\\{pond-mode-map}"
  (interactive)
  (kill-all-local-variables)

  (use-local-map pond-mode-map)

  (setq major-mode 'pond-mode)
  (setq mode-name "Pond")

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
  ;;       (not pond-never-start-kernel-with-mode)
  ;;       (or pond-always-start-kernel-with-mode
  ;;           (y-or-n-p "Start a Pond kernel for evaluations? "))
  ;;       ) ; end of logical expression
  ;;   (progn
  ;;     (pond-internal-start-kernel (current-buffer))
  ;;     (if pond-split-on-startup
  ;;         (if (not (get-buffer-window "*Pond-Kernel*"))
  ;;             (pond-start-split-screen)
  ;;           ) ; end if the buffer is not visible
  ;;       ) ; end if split on startup is true
  ;;     ) ; end of progn
  ;;   (message "You can start a Pond kernel with C-c r or M-x pond-restart-kernel.")
  ;;   ) ; end if

  (run-mode-hooks 'pond-mode-hook)
  )

(defun pond ()
  "Start a Pond process in a new buffer."
  (interactive)
  (let ((oldval pond-always-start-kernel-with-mode))
    (setq pond-always-start-kernel-with-mode t)
    (switch-to-buffer (generate-new-buffer "tmp.pd"))
    (pond-mode)
    (setq pond-always-start-kernel-with-mode oldval)
    ) ; end let
  )

(defun pond-execute (arg)
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

(defun pond-execute-all (arg)
  "Executes the whole .pd file"
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

(provide 'pond)
