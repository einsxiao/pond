;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Name: pond-kernel.el
;;; Version: 1.0©2015
;;; Author: Xiao,Hu 
;;; Intro: pond-kernel mode for emacs to corporate with platform Pond.

;; the buffer *Pond-Kernel* will set to mode 'pond-kernel-mode after it being
(defvar pond-kernel-status () "A word or two describing the state of the Pond kernel.") 
(defvar pond-kernel-workbuf () "An association list connecting Pond processes with working buffers.") 
(defvar pond-waiting-for-abort-message nil "A set of Pond processes which are waiting for an interrupt message.")
(defvar pond-waiting-for-abort-message2 nil "A set of Pond processes which are waiting for the second part of an interrupt message." )
(defvar pond-kernel-mode-map () "Keymap used in Pond-Kernel mode.")
(defvar pond-kernel-input-list () "List of kernel input sequence.")
(defvar pond-kernel-temp-input nil "A variable to record temp input in pond-kernel buffer.")

;;if a kernel is exist and the kernel buffer can be seen, just do nothing.
;;if a kernel is exit but the kernel buffer can not be seen, just split the screen to make the kernel buffer visible.
;;if no kernel is exit, start the kernel and split the screen to make the kernel buffer visible.
(global-set-key "\C-cs" 'pond-start-split-screen)
(defun Pond ()
  "another name to pond-start-split-screen"
  (interactive)
  (pond-start-split-screen))

(if pond-kernel-mode-map
    ()
  (setq pond-kernel-mode-map (make-sparse-keymap))
  (define-key pond-kernel-mode-map (kbd "RET") 'pond-kernel-execute)
  (define-key pond-kernel-mode-map "\C-ca" 'pond-abort-calculation)
  (define-key pond-kernel-mode-map "\C-cr" 'pond-restart-kernel)
  (define-key pond-kernel-mode-map "\C-ck" 'pond-kill-this-kernel)
  (define-key pond-kernel-mode-map "\C-d" 'pond-kill-this-kernel)
  (define-key pond-kernel-mode-map "\177" 'backward-delete-char-untabify)
  (define-key pond-kernel-mode-map ")" 'pond-evlectric-paren)
  (define-key pond-kernel-mode-map "]" 'pond-evlectric-braket)
  (define-key pond-kernel-mode-map "}" 'pond-evlectric-brace) 
  (define-key pond-kernel-mode-map (kbd "<left>") 'pond-kernel-left)
  (define-key pond-kernel-mode-map (kbd "<right>") 'pond-kernel-right)
  (define-key pond-kernel-mode-map (kbd "<up>") 'pond-kernel-up)
  (define-key pond-kernel-mode-map (kbd "<down>") 'pond-kernel-down)
  (evil-define-key 'insert pond-kernel-mode-map (kbd "<home>") 'pond-kernel-home)
  (evil-define-key 'normal pond-kernel-mode-map "i" 'pond-kernel-evil-insert-state)
  (evil-define-key 'normal pond-kernel-mode-map "a" 'pond-kernel-evil-append)
 )

(defvar pond-evode-abbrev-table nil
  "Abbrev table in use in pond-evode buffers."
  )

(defvar pond-evode-syntax-table nil "")
(if (not pond-evode-syntax-table)
  (let ((i 0))
    (setq pond-evode-syntax-table (make-syntax-table))

    ;; white space
    (modify-syntax-entry ?  " " pond-evode-syntax-table)  
    (modify-syntax-entry ?\t " " pond-evode-syntax-table)
    (modify-syntax-entry ?\f " " pond-evode-syntax-table)
    (modify-syntax-entry ?\n " " pond-evode-syntax-table)
    (modify-syntax-entry ?\^m " " pond-evode-syntax-table)

    ;; pure parens
    (modify-syntax-entry ?[ "(]" pond-evode-syntax-table)
    (modify-syntax-entry ?] ")[" pond-evode-syntax-table)
    (modify-syntax-entry ?{ "(}" pond-evode-syntax-table)
    (modify-syntax-entry ?} "){" pond-evode-syntax-table)

    ;; punctuation
    (modify-syntax-entry ?= "." pond-evode-syntax-table)
    (modify-syntax-entry ?: "." pond-evode-syntax-table)
    (modify-syntax-entry ?% "." pond-evode-syntax-table)
    (modify-syntax-entry ?< "." pond-evode-syntax-table)
    (modify-syntax-entry ?> "." pond-evode-syntax-table)
    (modify-syntax-entry ?& "." pond-evode-syntax-table)
    (modify-syntax-entry ?| "." pond-evode-syntax-table)
    (modify-syntax-entry ?_ "." pond-evode-syntax-table)
    (modify-syntax-entry ?/ "." pond-evode-syntax-table)
    (modify-syntax-entry ?! "." pond-evode-syntax-table)
    (modify-syntax-entry ?@ "." pond-evode-syntax-table)
    (modify-syntax-entry ?\' "." pond-evode-syntax-table)
    (modify-syntax-entry ?# "." pond-evode-syntax-table)


    ;; comments and parens
    ;;; style 1 same as mathematica
    (modify-syntax-entry ?\( ". 1" pond-evode-syntax-table)
    (modify-syntax-entry ?\) ". 4" pond-evode-syntax-table)
    (modify-syntax-entry ?* ". 23" pond-evode-syntax-table)
    ;;; style 2 # is newly defined and # can be nested in (**)
    (modify-syntax-entry ?\# "< b" pond-evode-syntax-table)
    (modify-syntax-entry ?\n "> b" pond-evode-syntax-table)

    ;; quotes
    (modify-syntax-entry ?\\ "\\" pond-evode-syntax-table)
    (modify-syntax-entry ?\" "\"" pond-evode-syntax-table)

    ;; for Pond numbers, the following would be better as
    ;; parts of symbols
    (modify-syntax-entry ?- "_" pond-evode-syntax-table)
    (modify-syntax-entry ?. "_" pond-evode-syntax-table)
    (modify-syntax-entry ?\` "_" pond-evode-syntax-table)
    (modify-syntax-entry ?^ "_" pond-evode-syntax-table)

    (modify-syntax-entry ?$ "_" pond-evode-syntax-table)
    (modify-syntax-entry ?+ "_" pond-evode-syntax-table)

    ;; create an abbrev table for pond-kernel mode
    (define-abbrev-table 'pond-evode-abbrev-table ())

    ) ; end of let
  )

(defvar pond-font-lock-keywords
  '(
    ;;In and out
    ("^In\[[0-9]+\]:=" . font-lock-keyword-face)
    ("^Out\[[0-9]+\]=" . font-lock-keyword-face)
    ("^Out\[[0-9]+\]//[A-Za-z][A-Za-z0-9]*=" . font-lock-keyword-face)
    ("\\([A-Za-z][A-Za-z0-9]*\\)[ \t]*[\[][ \t]*[\[]" 1 "default")
    ;; function name
    ("\\([A-Za-z][A-Za-z0-9]*\\)[ \t]*[\[]" 1 font-lock-function-name-face)
    ("//[ \t\f\n]*\\([A-Za-z][A-Za-z0-9]*\\)" 1 font-lock-function-name-face)
    ("\\([A-Za-z][A-Za-z0-9]*\\)[ \t\f\n]*/@" 1 font-lock-function-name-face)
    ("\\([A-Za-z][A-Za-z0-9]*\\)[ \t\f\n]*//@" 1 font-lock-function-name-face)
    ("\\([A-Za-z][A-Za-z0-9]*\\)[ \t\f\n]*@@" 1 font-lock-function-name-face)
    ("_[) \t]*\\?\\([A-Za-z][A-Za-z0-9]*\\)" 1 font-lock-function-name-face)
    ;;;;;;;;;;;;;;
    ("\\(&&\\)" 1 "default")
    ("\\\\[[A-Za-z][A-Za-z0-9]*\]" . font-lock-constant-face )
    ;;; rule

    ;;; pattern
    ("[A-Za-z][A-Za-z0-9]*$[A-Za-z0-9]*___" . font-lock-preprocessor-face)
    ("$[A-Za-z0-9]+___" . font-lock-preprocessor-face)
    ("[A-Za-z][A-Za-z0-9]*___" . font-lock-preprocessor-face)

    ("[A-Za-z][A-Za-z0-9]*$[A-Za-z0-9]*__" . font-lock-preprocessor-face)
    ("$[A-Za-z0-9]+__" . font-lock-preprocessor-face)
    ("[A-Za-z][A-Za-z0-9]*__" . font-lock-preprocessor-face)

    ("[A-Za-z][A-Za-z0-9]*$[A-Za-z0-9]*_" . font-lock-preprocessor-face)
    ("$[A-Za-z0-9]+_" . font-lock-preprocessor-face)
    ("[A-Za-z][A-Za-z0-9]*_" . font-lock-preprocessor-face)
    ("_" . font-lock-preprocessor-face)

    ;; about $ & variable and pure function
    ("[A-Za-z][A-Za-z0-9]*$[A-Za-z0-9]*" . font-lock-variable-name-face )
    ("$[A-Za-z0-9]+" . font-lock-variable-name-face )
    ("$_[1-9][0-9]*" . font-lock-type-face )
    ("$_" . font-lock-type-face )
    ("&" . font-lock-function-name-face)
    ;;; String face
    ("\\\".*?\\\"" . font-lock-string-face )
    ("'.*?'" . font-lock-string-face )
    ;;; number face
    ("[0-9]+" . font-lock-constant-face )
    ("[0-9]+\.[0-9]+" . font-lock-constant-face )
    ;;; compound face
    ;;;("[A-Za-z][A-Za-z0-9]*\.\\\(?:[A-Za-z][A-Za-z0-9]*\.\\\)*" . font-lock-type-face )
    ;;; keywords
    ("\\<\\(Null\\|GetModule\\)\\>" . font-lock-keyword-face)

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ("\\([A-Za-z0-9]+\\)[ \t]*\\->" 1 font-lock-type-face )
    ("<<[ \t\f\n]*[A-Za-z][A-Za-z0-9]*`[ \t\f\n]*[A-Za-z][A-Za-z0-9]*[ \t\f\n]*`"
     . font-lock-type-face )
    ("[A-Za-z][A-Za-z0-9]*::[A-Za-z][A-Za-z0-9]*" . font-lock-warning-face)
    ("\\[Calculating\\.\\.\\.\\]" . font-lock-warning-face)
    ("\\[Pond.*\\]" . font-lock-warning-face)
    ("^Interrupt>" . font-lock-warning-face)
    ("-Graphics-" . font-lock-type-face)
    ("-DensityGraphics-" . font-lock-type-face)
    ("-ContourGraphics-" . font-lock-type-face)
    ("-SurfaceGraphics-" . font-lock-type-face)
    ("-Graphics3D-" . font-lock-type-face)
    ("-GraphicsArray-" . font-lock-type-face)
    ("-Sound-" . font-lock-type-face)
    ("-CompiledCode-" . font-lock-type-face)
    )
  )

(defun pond-set-buffer-read-only (backward-amount)
  "set buffer to be read-only while appending chars is allowed."
  (interactive)
  (setq inhibit-read-only t)
  ;; remove possible related properties
  (remove-text-properties
   (point-min) (point-max)
   '(read-only t front-sticky t rear-nonsticky t))
  (add-text-properties (point-min) (max (- (point-max) backward-amount) (point-min)) '(read-only t)) ; text read-only
  (add-text-properties (point-min) (min 2 (point-max)) '(front-sticky t)) ;unable to add at head
  (add-text-properties (max (point-min) (- (point-max) 1)) (point-max) '(rear-nonsticky t)) ;able to append
  (setq inhibit-read-only nil)
  (setq pond-kernel-input-list-pointer -1)
  (if (fboundp 'evil-insert-state)
      (evil-insert-state))
  (goto-char (point-max))
  )

(defun pond-kernel-mode ()
  "Major mode for Pond Kernel in Emacs."
  (interactive)
  (kill-all-local-variables)
  (use-local-map pond-kernel-mode-map)
  (setq major-mode 'pond-kernel-mode)
  (setq mode-name "Pond-Kernel")
  ;;added lines
  (setq pond-kernel-status "not ready")
  (setq mode-line-process '(" is " pond-kernel-status "."))

  (make-local-variable 'font-lock-defaults)
  (setq font-lock-defaults '(pond-font-lock-keywords nil t))

  (setq local-abbrev-table pond-evode-abbrev-table)
  (set-syntax-table pond-evode-syntax-table)

  (make-local-variable 'indent-line-function)
  (setq indent-line-function 'pond-indent-line)

  (make-local-variable 'comment-start)
  (setq comment-start "(*")
  (make-local-variable 'comment-end)
  (setq comment-end " *)")
  (make-local-variable 'comment-start-skip)
  (setq comment-start-skip "\\(//+\\|/\\*+\\)\\s *\\#")

  ;;(setq comment-multi-line t)
  ;;(setq-local comment-start-skip "/\\*+[ \t]*")
  ;;(setq-local comment-end-skip "[ \t]*\\*+/")
  ;;(setq-local parse-sexp-ignore-comments t)

  (make-local-variable 'paragraph-start)
  (setq paragraph-start (concat page-delimiter "\\|$"))
  (make-local-variable 'paragraph-separate)
  (setq paragraph-separate paragraph-start)

  (make-local-variable 'font-lock-defaults)
  (setq font-lock-defaults '(pond-font-lock-keywords nil t))

  (make-local-variable 'kill-buffer-hook)
  (setq kill-buffer-hook 'pond-kill-this-kernel)

  (pond-set-buffer-read-only 0)
  (goto-char (point-max))

  (run-mode-hooks 'pond-kernel-mode-hook)
  )


(defun pond-kernel ()
  "Start a Pond process."
  (interactive)
  (let ((oldval pond-kernel-always-start-kernel-with-mode))
    (setq pond-kernel-always-start-kernel-with-mode t)
    (switch-to-buffer (generate-new-buffer "tmp.ev"))
    (pond-kernel-mode)
    (setq pond-kernel-always-start-kernel-with-mode oldval)
    ) ; end let
  )

(defun pond-internal-start-kernel (workbuf)
  "Internal function for starting Pond kernels."
  (pond-cleanup-zombies)
  (setq workbuf "*Pond-Kernel*")
  (let ((kernel) (oldbuf))
    (if (rassoc workbuf pond-kernel-workbuf)
        (progn
          ;;(message "There is already a kernel running!" )
          (pond-kernel-from-workbuf workbuf)
          )
      (progn
        (message "Starting Pond kernel...")
        (setq kernel (start-process
                      "Pond-Kernel"
                      "*Pond-Kernel*"
                      pond-command-line))
        (message "Starting Pond kernel... done!")

        (setq oldbuf (current-buffer))
        (set-buffer (process-buffer kernel))
        (pond-kernel-mode)
        (set-buffer oldbuf)

        (setq pond-kernel-workbuf
              (append pond-kernel-workbuf
                      (cons (cons kernel workbuf) nil)
                      ) ; end append
              ) ; end setq

        (set-process-filter kernel 'pond-kernel-filter)
        (set-process-sentinel kernel 'pond-kernel-sentinel)

        (if (processp kernel) (setq pond-kernel-status "starting up"))
        kernel
        ) ; end of "starting Pond" progn
      ) ; end if process already exists
    ) ; end let
  )

(defun pond-kill-this-kernel ()
  "Kills the Pond kernel."
  (interactive)
  (let ((kernel (pond-kernel-from-workbuf (current-buffer)))
	(isreal))
    (setq isreal (processp kernel))
    (pond-kill-kernel kernel)
    (if isreal (message "Pond process killed."))
    (setq pond-kernel-status "not ready")
    ) ; end let
  )

(defun pond-kill-kernel (kernel)
  "Internal function for killing a Pond kernel."
  (if (processp kernel)
      (progn
        (setq pond-kernel-workbuf
              (delete (assoc kernel pond-kernel-workbuf)
                      pond-kernel-workbuf)
              ) ; end of setq

        (delete-process kernel)

        ) ; end of progn
    (pond-cleanup-zombies)
    ) ; end if kernel is still alive
  )

(defun pond-restart-kernel ()
  "Restarts the Pond kernel."
  (interactive)
  (let ((kernel (pond-kernel-from-workbuf (current-buffer))))
    (if (processp kernel)
        (pond-kill-kernel kernel)
      nil)
    ) ; end of let
  (pond-internal-start-kernel (current-buffer))
  )

(defun math()
  "Starts the Pond Kernel instantly"
  (interactive)
  (pond-start-kernel))
  
(defun pond-start-kernel ()
  "Starts the Pond Kernel instantly"
  (interactive)
  (let ((kernel (pond-kernel-from-workbuf (current-buffer))))
    (if (processp kernel)
        (if (get-buffer-window  (process-buffer kernel))
            (message "*Pond-Kernel* is already running")
          (set-window-buffer
           (frame-selected-window) (process-buffer kernel))
          ) ; end if
      (progn ;when not start
        (pond-internal-start-kernel (current-buffer))
        ); end of progn
      ) ; end of if processp kernel
    ;; show the *Pond-Kernel* at current window
    (switch-to-buffer "*Pond-Kernel*")
    ) ; end of let
  )



(defun pond-find-expression-end ()
  "test if line containing position pos is connected with line forward and return the end position of the last line"
  (interactive )
  (save-excursion
    (let (
          (in-comment nil) (in-string nil) (double-string)
          (depth 0)
          (operator-connected nil)
          (start-pos) (end-pos nil) (last-char nil)
          (line-end nil) (syntax-error nil) )
      (setq start-pos (point))
      (while (not (or line-end end-pos))
        (cond
         ( in-string
           (if (or (and double-string (equal (char-after (point)) ?\"))
                   (and (not double-string) (equal (char-after (point)) ?\')) ) ;end of or
               (setq in-string nil last-char nil)
             ) ; end of if: to end a string 
           ) ; end of in-string
         ( in-comment
           (if (and (equal (char-after (point)) ?\*) (equal (char-after (+ (point) 1)) ?\)))
               (progn (forward-char) (setq in-comment nil)));end of if
           ) ; end of in-comment
         ( (equal (char-after (point)) ?\" ) ; enter string enclosed by "
           (setq in-string t double-string t last-char nil) ) 
         ( (equal (char-after (point)) ?\' ) ; enter string enclosed by '
           (setq in-string t double-string nil last-char nil) ) 
         ( (equal (char-after (point)) ?\# ) ; skip comment start by # and end with \n
           (move-end-of-line 1)
           (backward-char)
           )
         ( (or (equal (char-after (point)) ?\] )
               (equal (char-after (point)) ?\) )
               (equal (char-after (point)) ?\} ) )
           (setq depth (- depth 1) last-char nil)
           (if (< depth 0) ; never take previous line into account
               (setq end-pos (- (point) 1) line-end t)
             nil
             ) ; end  ] or ) or } kind of things
           ) ; end if
         ( (or (equal (char-after (point)) ?\[ )
               (equal (char-after (point)) ?\( )
               (equal (char-after (point)) ?\{ ) )
           (setq depth (+ depth 1) last-char (char-after (point)) )
           ) ; end [ ( {
         ( (equal (char-after (point)) ?\\ )
           (forward-char) 
           ) ; no matter what kind of escape char including line connector \\n; just forward a char
         ( (equal (char-after (point)) ?\n )
           (setq line-end t)
           ) ; end of \n
         ( (equal (char-after (point)) ?\t ) nil)
         ( (equal (char-after (point)) ?\  ) nil)
         ( t
           (setq last-char (char-after (point)))
           (if (or (equal (char-after (point)) ?+)
                   (equal (char-after (point)) ?-))
               (if (equal (char-after (point)) (char-before (point)))
                   (setq last-char nil) ) )
           ); end of normal char; default term of cond
         ) ; end of cond 
        ;; a char is processed
        (if (or line-end end-pos)
            (progn
              (if (and last-char
                       (or
                        (equal last-char ?+ ) (equal last-char ?- ) (equal last-char ?* ) (equal last-char ?/ )
                        (equal last-char ?. ) (equal last-char ?^ ) (equal last-char ?@ ) (equal last-char ?= )
                        (equal last-char ?% ) ) ; end of or
                       ) ; end of and 
                  (setq operator-connected t)
                (setq operator-connected nil)
                ) ; end of if
              (if operator-connected
                  (if  end-pos  ; like +] report error
                      (progn (setq syntax-error t)
                             (message "Expression can not be completed! Please check!") )
                    (progn (forward-char) (setq line-end nil) ) ; case of line-end
                    ) ; end of operator-connected being true
                (if line-end ; find the end of the expression
                    (if (> depth 0)
                        (progn (forward-char) (setq line-end nil))
                      (setq end-pos (point)) ; depth == 0 expression end
                      ) ; end of if
                  ) ; end of if
                ) ; end of if 
              ); end of progn
          (forward-char) ; else of if 
          ); end of if
        (if (equal (point-max) (point))
            (setq end-pos (point))
          )
        ) ; end of while; break when line end
      ;;(princ (format "start-pos: %d  end-pos: %d\n" start-pos end-pos))
      ;;(princ (concat "line-end: " (if line-end "t" "nil") "\n")) 
      ;;(princ (concat (buffer-substring start-pos end-pos) "\n"))
      (if syntax-error -1
        end-pos)
      ) ; end of let
    ) ; end of save-excursion
  ) ; end of defun

(defun pond-kernel-execute (arg)
  "Executes expression entered in the Pond kernel buffer.
   If a \ is at the end of the line continue enter at next line." 
  (interactive "P")
  (let ((kernel (pond-kernel-from-workbuf (current-buffer))) )
    (if (processp kernel)
        (let ((start) (end) (input) )
          ;; If a "\"(line succcesive exist), continue input at next line
          (if (equal (save-excursion
                       (end-of-buffer) (skip-chars-backward " \t\n")
                       (char-before (point)))
                     ?\\)
              (progn
                (end-of-buffer)
                (insert-char ?\n)
                ) ; end of the first part of if
            (progn 
              ;; start from expr after "In[xxx]:=", judged by read-only properties
              (save-excursion
                (end-of-buffer)
                (while (and (> (point) (point-min)) (not (get-char-property (point) 'read-only))) (backward-char))
                (forward-char)
                (setq start (point))
                )
              ;; move point to end of the line
              (end-of-buffer) (skip-chars-backward " \t") (setq end (point))
              (if (> (- end start) 0) 
                  (progn
                    ;; get the input
                    (setq input (buffer-substring start end))
                    (goto-char (point-max)) (insert-char ?\n)
                    (pond-evxecute-expression
                     (replace-regexp-in-string "\n" "\\\n" input t t)
                     nil)
                    (goto-char (point-max))
                    ) ; end of progn
                (progn ; when input is empty, send an empty input to make the return key action for the purpose of improving user experience
                  (setq input "\n")
                  (goto-char (point-max)) (insert-char ?\n)
                  (pond-evxecute-expression
                   (replace-regexp-in-string "\n" "\\\n" input t t)
                   nil)
                  (goto-char (point-max))
                  ); end of progn
                ) ; end of if input is empty
              ) ; end of the second part of if
            ) ; end of if
          ) ; end let
      (error "This buffer has no Pond kernel! Restart kernel with C-c-r")
      ) ; end of if
    ) ; end of let
  )

(defun pond-evxecute-expression (expr display)
  "Executes an expression with the Pond kernel.
    if display is t, display expr on kernel file"
  (interactive)
  (if (null expr)
      (error "null expression when call pond-evxecute-expression")
    (let ((kernel (pond-kernel-from-workbuf (current-buffer))) )
      (if (processp kernel)
          (let ( (kernel) (input) (oldbuf (current-buffer))
                 (tmpbuf (generate-new-buffer "*pond-temp*")))
            ;; get the input
            (setq input expr)
            ;; record input history
            (setq pond-kernel-input-list (cons input pond-kernel-input-list))
            (if (>= (length pond-kernel-input-list) pond-kernel-input-max-amount)
                (nbutlast pond-kernel-input-list))
            ;; format it properly
            (set-buffer tmpbuf)
            (insert input)
            ;; Resolving the disagreement between Emacs and
            ;; Pond about the use of "\[" as an open paren
            (setq input (format "%s\n" (buffer-substring (point-min) (point-max))))
            (beginning-of-buffer) (while (search-forward "\\(" nil t) (replace-match "(" nil t))
            (beginning-of-buffer) (while (search-forward "\\)" nil t) (replace-match ")" nil t))
            (beginning-of-buffer) (while (search-forward "\\[" nil t) (replace-match "[" nil t))
            (beginning-of-buffer) (while (search-forward "\\]" nil t) (replace-match "]" nil t))
            (beginning-of-buffer) (while (search-forward "\\{" nil t) (replace-match "{" nil t))
            (beginning-of-buffer) (while (search-forward "\\}" nil t) (replace-match "}" nil t))
            (beginning-of-buffer)
            (while (search-forward "\\\"" nil t)
              (let ((number-of-quotes) (characters (append (buffer-substring (point-min) (point)) nil)))
                (setq number-of-quotes (1- (- (length characters) (length (delq ?\" characters)))))
                (if (= (% number-of-quotes 2) 0)
                    (replace-match "\"" nil t)
                  (replace-match "" nil t)
                  ) ; end if
                ) ; end let
              ) ; end while

            (let ((number-of-quotes) (characters (append (buffer-substring (point-min) (point-max)) nil)))
              (setq number-of-quotes (- (length characters) (length (delq ?\" characters))))
              (if (= (% number-of-quotes 2) 1)
                  (progn
                    (set-buffer oldbuf)
                    (kill-buffer tmpbuf)
                    (error "The quotes are not balanced")
                    ) ; end progn
                ) ; end if
              ) ; end let

            (if (not (= (car (parse-partial-sexp (point-min) (point-max))) 0))
                (progn
                  (set-buffer oldbuf)
                  (kill-buffer tmpbuf)
                  (error "The parentheses are not balanced")
                  ) ; end progn
              )
            (set-buffer oldbuf)
            (kill-buffer tmpbuf)
            ;; insert the input and marks into the queue corresponding
            ;; to this kernel
            (setq kernel (pond-kernel-from-workbuf oldbuf))

            (set-buffer (process-buffer kernel))
            (goto-char (point-max))
            (if display
                (progn ; expr evaluated is not from kernel buffer
                  ;; record chars already on the kernel buffer
                  (while (and (> (point) (point-min)) (not (get-char-property (point) 'read-only)))
                    (backward-char))
                  (forward-char)
                  (setq pond-kernel-temp-input (filter-buffer-substring (point) (point-max) t))
                  (insert input)
                  )
              )
            (pond-set-buffer-read-only 0)
            (goto-char (point-max))
            (set-buffer oldbuf)

            ;; send the command to Pond
            (process-send-string kernel input)
            (setq pond-kernel-status "not ready")
            ) ; end let
        (error "No Pond kernel exist!")
        ) ; end of if
      ) ; end of let
    ) ; end of if
  ) ; end of defun


(defun pond-kernel-filter (process output)
  "Puts the Pond output where it needs to go."
  (let ((oldbuf (current-buffer)) (maybe-more-output t))
    (progn
      (set-buffer (pond-workbuf-from-kernel process))
      (setq pond-kernel-status "ready")
      (set-buffer oldbuf)
      ;; always put new output at the end
      (set-buffer (marker-buffer (process-mark process)))
      (goto-char (point-max))
      (insert output)
      (pond-set-buffer-read-only 0)
      (goto-char (point-max))
      (if pond-kernel-temp-input ; put temp-input of the kernel behide newly In[xxx]:=
          (if (save-excursion ; test whether a input beginning
                (beginning-of-line)
                (looking-at-p "^In\[[0-9]+\]:=$"))
              (progn
                (end-of-line)
                (insert pond-kernel-temp-input)
                (setq pond-kernel-temp-input nil)
                ) ; end of progn
            ) ; end of if
        ) ; end of if
      ) ; end of progn
    (set-buffer oldbuf)
    ) ; end of let
  )

(defun pond-kernel-sentinel (process event)
  (let ((oldbuf (current-buffer)))
    (set-buffer (process-buffer process))
    (goto-char (point-max))
    (insert (format "\n * * * Pond %s" event))
    (delete-char -1)
    (insert ". * * *\n\n")
    (pond-set-buffer-read-only 0)
    (goto-char (point-max))
    (set-buffer oldbuf)
    ) ; end of let
  
  (pond-cleanup-zombies)
  )

(defun pond-abort-calculation ()
  "Abort the current Pond calculation, if there is any."
  (interactive)
  (let ((kernel (pond-kernel-from-workbuf (current-buffer))))
    (interrupt-process kernel)
    (setq pond-waiting-for-abort-message
	  (cons kernel pond-waiting-for-abort-message)
	  ) ; end setq
    ) ; end of let
  )

(defun pond-kernel-from-workbuf (workbuf)
  "Internal function for getting the kernel from a working buffer."
  (setq workbuf "*Pond-Kernel*")
  (car (rassoc workbuf pond-kernel-workbuf))
  )

(defun pond-workbuf-from-kernel (kernel)
  "Internal function for getting the working buffer from a kernel."
  (cdr (assoc kernel pond-kernel-workbuf))
  )


(defun pond-cleanup-zombies ()
  "Removes items from \"pond-kernel-workbuf\" that no longer exist."
  (interactive)

  ;; first do it for pond-kernel-workbuf
  (let ((tmp (copy-alist pond-kernel-workbuf)) (i 0))
    (setq pond-kernel-workbuf nil)
    (while (< i (length tmp))
      (if (string= "run" (process-status (car (nth i tmp))))
          (setq pond-kernel-workbuf
                (append pond-kernel-workbuf (cons (nth i tmp) nil))
                ) ; end of setq
        nil
        ) ; end if
      (setq i (1+ i))
      ) ; end while
    ) ; end let
  )

(defun pond-indent-determine-in-comment ()
  "Returns the beginning of the comment, or nil."
  (save-excursion
    (let ((here (point)) (no-open nil) (first-open) (no-close nil) (first-close))

      (if (search-backward "(*" nil t)
	  (setq first-open (point))
	(setq no-open t)
	) ; end if

      (goto-char here)
      (if (search-backward "*)" nil t)
	  (setq first-close (point))
	(setq no-close t)
	) ; end if
      
      (cond ((and no-open no-close) nil)
	    ((and (not no-open) no-close) first-open)
	    ((and no-open (not no-close)) nil)
	    ((and (not no-open) (not no-close))
	     (if (> first-open first-close) first-open nil)
	     )
	    ) ; end cond
      ) ; end let
    ) ; end excursion
  )

(defun pond-indent-determine-unbalanced ()
  "Returns the beginning of the open paren or nil. Assumes not in
comment."
  (save-excursion
    (let ((toplevel nil) (home nil))
      (condition-case nil
	  (while (not home)
	    (up-list -1)
	    (if (and (<= (+ (point) 2) (point-max))
		     (string=
		      (format "%s" (buffer-substring (point) (+ (point) 2)))
		      "(*")
		     ) ; end of and
		(setq home nil)
	      (setq home t)
	      ) ; end if this open paren is the start of a comment
	    ) ; end while looking for an unbalanced open paren
	(error (setq toplevel (point)))
	) ; end condition-case
      (if toplevel nil (point))
      ) ; end let
    ) ; end excursion
  )

(defun pond-stepthrough-comments ()
  "Moves the point backward through comments and non-eoln whitespace."
  (let ((home nil))
    (while (not home)
      (if (and (>= (- (point) 2) (point-min))
               (string= (string (char-after (+ (point) 0))) "\n")
               ) ; end of and ---  
          (if (search-backward "#" (line-beginning-position) t)
              (setq home nil)
            nil); end if comment of style # occur
        ) ; end if comment # 
      (skip-chars-backward " \t")
      (setq home t) ; tenative assumtion
      (if (and (>= (- (point) 2) (point-min))
               (string=
                (format "%s" (buffer-substring (- (point) 2) (point)))
                "*)")
               ) ; end of and
          (if (search-backward "(*" nil t)
              (setq home nil)
            nil) ; end if comment has a beginning
        ) ; end if we stopped at the end of a comment
      ) ; end loop between comments and whitespace
    ) ; end of let
  )

(defun pond-indent-line-emptyp ()
  "Returns t if the line the point is on is empty, nil if not."
  (save-excursion
    (beginning-of-line)
    (skip-chars-forward " \t")
    (looking-at "[\f\n]")
    ) ; end excursion
  )

(defun pond-indent-determine-prevline ()
  "Returns the meaningful end of the previous line (is it a
semicolon?), under the assumtion that you're not in a comment or
unbalanced parens."
  (save-excursion
    (let ((home nil) (meaningful-end))
      (while (not home)
        (beginning-of-line)
        (if (= (point) (point-min))
            (progn ; There's nowhere to go. You're quite done.
              (setq meaningful-end nil)
              (setq home t)
              ) ; end of progn
          (progn

            (backward-char 1)
            (pond-stepthrough-comments)
            
            (if (pond-indent-line-emptyp)
                (progn ; we're done, there is no previous line
                  (setq meaningful-end nil)
                  (setq home t)
                  ) ; end progn
              (progn
                (setq meaningful-end (point))
                (beginning-of-line)
                (if (= meaningful-end (point))
                    (setq home nil) ; there was nothing on this line but
                                        ; comments
                  (setq home t) ; this is a good line
                  )
                ) ; end progn
              ) ; end if-else line empty

            ) ; end line empty progn
          ) ; end line empty if-else

        ) ; end while
      
      meaningful-end
      ) ; end let
    ) ; end excursion
  )

(defun pond-indent-determine-indent ()
  "Returns the indentation of the line the point is on."
  (save-excursion
    (beginning-of-line)
    (skip-chars-forward " \t")
    (current-column)
    ) ; end excursion
  )

(defun pond-indent-calculate (start)
  (save-excursion
    (goto-char start)
    (beginning-of-line)
    (skip-chars-forward " \t")

    (let ((start-char) (start-close-paren ? ) (in-comment) (in-unbalanced) (prevline))
      (if (not (= (point) (point-max)))
          (progn
            (setq start-char (char-after))
            (cond
             ((= start-char ?\)) (setq start-close-paren ?\())
             ((= start-char ?\]) (setq start-close-paren ?\[))
             ((= start-char ?}) (setq start-close-paren ?{))
             ) ; end of cond
            ) ; end of progn
        nil
        ) ; end if you're not at the very end of the buffer

      (setq in-comment (pond-indent-determine-in-comment))
      (if in-comment ; in-comment = the position of the opening of the comment
          (let ((tmp (+ in-comment 2)) (tmp-column))
            (goto-char tmp)
            (setq tmp-column (current-column))

            (skip-chars-forward " \t")
            (if (looking-at "[\f\n]") ; first comment line has nothing
                                        ; but "(*"
                (1+ tmp-column) ; return one space after the "(*"
              (current-column)
              ) ; end if
            ) ; end let in-comment

        (progn ; from now on, you're not in a comment
          (setq in-unbalanced (pond-indent-determine-unbalanced))
          (if in-unbalanced ; in-unbalanced = the opening paren
              (progn
                (goto-char in-unbalanced)
                (if (= (char-after) start-close-paren)
                    (current-column)
                  (let ((tmp in-unbalanced)) 
                    (forward-char 1)
                    (skip-chars-forward " \t")
                    (if (looking-at "[\f\n]")
                        (+ (pond-indent-determine-indent) 4)
                      (current-column)
                      ) ; end if unbalanced-paren ends the line
                    ) ; end let unbalanced-paren isn't immediately matched
                  ) ; end if immediate match
                ) ; end progn unbalanced-paren
            
            (progn ; from now on, you're not in a comment or
                                        ; unbalanced paren (you're at toplevel)
              (setq prevline (pond-indent-determine-prevline))
              (if prevline
                  (progn ; prevline = end of the last line
                    (goto-char prevline)
                    (if (= (char-before) ?\;)
                        0 ; a fully top-level command
                      (if (or
                           (= (char-before) ?+)
                           (= (char-before) ?-)
                           (= (char-before) ?*)
                           (= (char-before) ?/)
                           (= (char-before) ?^)
                           (= (char-before) ?.)
                           (= (char-before) ?\%)
                           (= (char-before) ?\@)
                           (= (char-before) ?\,)
                           );end of or
                          4 ; operator need extra input within following lines
                        0) ; a continuation of a toplevel command
                      ) ; end if last line ended in a semicolon
                    ) ; end progn there was a last line
                0 ; if there's no previous line (in this execution
                                        ; block) don't indent
                ) ; end prevline if-else
              ) ; end at toplevel progn

            ) ; end unbalanced if-else
          ) ; end non-comment progn
        ) ; end in-comment if-else
      ) ; end outermost let
    ) ; end excursion
  )

(defun pond-indent-line ()
  "Indent current line as Pond code."
  (interactive)
  (let ((indent (pond-indent-calculate (point))) shift-amt beg end
        (pos (- (point-max) (point))))
    (beginning-of-line)
    (setq beg (point))
    (skip-chars-forward " \t")
    (setq shift-amt (- indent (current-column)))
    (if (zerop shift-amt)
        nil
      (progn
        (delete-region beg (point))
        (indent-to indent)
        ) ; end of progn
      ) ; end if there is nothing to shift
    
    (if (> (- (point-max) pos) (point))
        (goto-char (- (point-max) pos))
      nil
      ) ; end if we need to move the cursor
    ) ; end of let
  )

(defun pond-evlectric-paren (arg)
  "Indents on closing a paren."
  (interactive "p")
  (let ((start (point)))
    (if (not arg) (setq arg 1) nil)
    (let ((i 0)) (while (< i arg) (insert ")") (setq i (1+ i))))
    (save-excursion
      (goto-char start)
      (skip-chars-backward " \t")
      (if (= (current-column) 0)
	  (pond-indent-line)
	nil
	) ; end if
      ) ; end excursion
    ) ; end let
  (blink-matching-open)
  )

(defun pond-evlectric-braket (arg)
  "Indents on closing a braket."
  (interactive "p")
  (let ((start (point)))
    (if (not arg) (setq arg 1) nil)
    (let ((i 0)) (while (< i arg) (insert "]") (setq i (1+ i))))
    (save-excursion
      (goto-char start)
      (skip-chars-backward " \t")
      (if (= (current-column) 0)
	  (pond-indent-line)
	nil
	) ; end if
      ) ; end excursion
    ) ; end let
  (blink-matching-open)
  )

(defun pond-evlectric-brace (arg)
  "Indents on closing a brace."
  (interactive "p")
  (let ((start (point)))
    (if (not arg) (setq arg 1) nil)
    (let ((i 0)) (while (< i arg) (insert "}") (setq i (1+ i))))
    (save-excursion
      (goto-char start)
      (skip-chars-backward " \t")
      (if (= (current-column) 0)
	  (pond-indent-line)
	nil
	) ; end if
      ) ; end excursion
    ) ; end let
  (blink-matching-open)
  )

(defun pond-kernel-left (arg)
  "Redefine <left>  key behavior when input within the Pond kernel buffer."
  (interactive "P")
  (if (> (point) (point-min))
      (if (not (get-char-property (- (point) 1) 'read-only))
          (backward-char)
        )
    )
  )

(defun pond-kernel-right (arg)
  "Redefine <right> key behavior when input within the Pond kernel buffer."
  (interactive "P")
  (if (< (point) (point-max))
      (forward-char)
    )
  )

(defun pond-kernel-up (arg)
  "Redefine <up> key behavior when input within the Pond kernel buffer."
  (interactive "P")
  (let ( (input) )
    (if (> (line-beginning-position) (point-min))
        (if (save-excursion (previous-line) (get-char-property (point) 'read-only))
            ;; get corresponding input from list pond-kernel-input-list
            (progn
              (setq pond-kernel-input-list-pointer (1+ pond-kernel-input-list-pointer))
              (if (< pond-kernel-input-list-pointer (length pond-kernel-input-list))
                  (progn ; find and insert the input item
                    (setq input (nth pond-kernel-input-list-pointer pond-kernel-input-list))
                    (goto-char (point-max))
                    (while (and (> (point) (point-min)) (not (get-char-property (point) 'read-only)))
                      (backward-char))
                    (forward-char)
                    (filter-buffer-substring (point) (point-max) t)
                    (insert input)
                    ) ; end of an item find and insert it while remove old input
                (progn ; if moved to the end of the list, relocate pointer to the very beginning
                  (setq pond-kernel-input-list-pointer -1)
                  (goto-char (point-max))
                  (while (and (> (point) (point-min)) (not (get-char-property (point) 'read-only)))
                    (backward-char))
                  (forward-char)
                  (filter-buffer-substring (point) (point-max) t)
                  ) ; end if pointer exceed maxium, and ...
                ) ; end of if
              ) ; end of progn
          (previous-line) ;if text at previous line can be edited, then just move to the previous line
          ) ; end of if
      ) ; end of if no previous line
    ) ; end of let
  ) ; end of defun

(defun pond-kernel-down (arg)
  "Redefine <down> key behavior when input within the Pond kernel buffer."
  (interactive "P")
  (let ( (input) )
    (if (= (line-end-position) (point-max))
        ;; get corresponding input from list pond-kernel-input-list
        (progn
          (setq pond-kernel-input-list-pointer (1- pond-kernel-input-list-pointer))
             (if (>= pond-kernel-input-list-pointer 0)
                 (progn ; find and insert the input item
                   (setq input (nth pond-kernel-input-list-pointer pond-kernel-input-list))
                   (goto-char (point-max))
                   (while (and (> (point) (point-min)) (not (get-char-property (point) 'read-only)))
                     (backward-char))
                   (forward-char)
                   (filter-buffer-substring (point) (point-max) t)
                   (insert input)
                   ) ; end of an item find and insert it while remove old input
               (progn ; if moved to the head list, clear the input
                 (setq pond-kernel-input-list-pointer -1)
                 (goto-char (point-max))
                 (while (and (> (point) (point-min)) (not (get-char-property (point) 'read-only)))
                   (backward-char))
                 (forward-char)
                 (filter-buffer-substring (point) (point-max) t)
                 ) ; end if pointer exceed maxium, and ...
               ) ; end of if
             ) ; end of progn
      (progn
        (next-line) ;if text at previous line can be edited, then just move to the previous line
        )
      ) ; end of if
    ) ; end of let
  ) ; end of defun

(defun pond-kernel-home (arg)
  "Redefine <home> key behavior when input within the Pond kernel buffer."
  (interactive "P")
  (beginning-of-line)
  (while (get-char-property (point) 'read-only)
    (forward-char))
  )

(defun pond-kernel-evil-insert-state (arg)
  "Redefine 'i' key behavior when input within the Pond kernel buffer in evil normal mode."
  (interactive "P")
  (while (get-char-property (point) 'read-only)
    (forward-char))
  (evil-insert-state)
  )

(defun pond-kernel-evil-append (arg)
  "Redefine 'a' key behavior when input within the Pond kernel buffer in evil normal mode."
  (interactive "P")
  (while (get-char-property (point) 'read-only)
    (forward-char))
  (if (< (point) (point-max)) (forward-char))
  (evil-insert-state)
  )

(defun pond-kernel-evil-backspace(arg)
  "Redefine 'backspace' key behavior when input within the Pond kernel buffer in evil insert mode."
  (interactive "P")
  (if (> (point) (point-min))
      (if (get-char-property (1- (point)) 'read-only)
          nil
        (delete-backward-char))
    nil
    )
  )


(defun pond-start-split-screen ()
  "Splits the screen into kernel buffer above, m buffer below. If no m buffer, just kernel buffer on"
  (interactive)
  (let ((kernel (pond-kernel-from-workbuf (current-buffer))))
    (if kernel
        (if (get-buffer-window  (process-buffer kernel))
            (message "*Pond-Kernel* is already running")
          (set-window-buffer
           (split-window nil nil 'above) (process-buffer kernel))
          ) ; end if
      (progn
        (pond-internal-start-kernel (current-buffer))
        (set-window-buffer
         (split-window nil nil 'above) (get-buffer "*Pond-Kernel*"))
        ) ; end of progn
      ) ; end if kernel
    ) ; end let
  )

;alist of 'buffer-name / timer' items
(defvar buffer-tail-alist nil)
(defun buffer-tail (name)
  "follow buffer tails"
  (cond
   ((or
     (equal (buffer-name (current-buffer)) name)
     (string-match "^ \\*Minibuf.*?\\*$" (buffer-name (current-buffer)))
     ) ; end of or
    ) ; first cond
   ((get-buffer name)
    (with-current-buffer (get-buffer name)
      ;;(goto-char (point-max))
      (let ((windows (get-buffer-window-list (current-buffer) nil t)))
        (while windows
          (set-window-point (car windows) (point-max))
          (with-selected-window (car windows) (recenter -4))
          (setq windows (cdr windows))
          ) ; end of while
        ) ; end let
      ) ; end with-current-buffer
    ) ; end second cond
   ) ; end of cond
  ) ; end of defun

(defun toggle-buffer-tail (name &optional force)
  "toggle tailing of buffer NAME. when called non-interactively,
    a FORCE arg of 'on' or 'off' can be used to to ensure a given state for buffer NAME"
  (interactive
   (list
    (cond
     ((if name name)
      (read-from-minibuffer 
       (concat
        "buffer name to tail" 
        (if buffer-tail-alist
            (concat " (" (caar buffer-tail-alist) ")")
          "") ; end if
        ": "
        ) ; end concat
       (if buffer-tail-alist (caar buffer-tail-alist))
       nil
       nil
       (mapcar (lambda (x) (car x)) buffer-tail-alist)
       (if buffer-tail-alist (caar buffer-tail-alist))
       ) ; end read-from-minibuffer
      ) ; end first condition
     nil ; second condition
     ); end cond
    ) ; end list
   ) ; end interactive
  (let ((toggle (cond (force force) ((assoc name buffer-tail-alist) "off") (t "on")) ))
    (if (not (or (equal toggle "on") (equal toggle "off"))) 
        (error "invalid 'force' arg. required 'on'/'off'") 
      (progn 
        (while (assoc name buffer-tail-alist) 
          (cancel-timer (cdr (assoc name buffer-tail-alist)))
          (setq buffer-tail-alist (remove* name buffer-tail-alist :key 'car :test 'equal)))
        (if (equal toggle "on")
            (add-to-list 'buffer-tail-alist (cons name (run-at-time t 1 'buffer-tail name))))
        (message "toggled 'tail buffer' for '%s' %s" name toggle)))))

(toggle-buffer-tail "*Pond-Kernel*" "on")

;; when no argument is followed when calling emacs, start a pond kernel
(if pond-startup-with-kernel-if-no-args
    ;; no argument followed when calling emacs
    (if (equal (length command-line-args) 1) 
        (pond-start-kernel)
      ); end of if
  ); end of if

;; dealing with -math option
;;

(provide 'pond-kernel)
