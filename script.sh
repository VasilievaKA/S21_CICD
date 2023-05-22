#!/usr/bin/expect -f
spawn ./move.sh

expect {
	"*fingerprint*" { send -- "yes\n" }
	"*password*" { send -- "123\n" }
}

expect EOF