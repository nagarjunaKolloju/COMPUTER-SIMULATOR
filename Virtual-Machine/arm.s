mov r0, 0
mov r14, 256
str r14, [r0, 0]
str r14, [r0, 1]
str r14, [r0, 2]
mov r14, 16389
str r14, [r0, 16388]
main:
	ldr r14,[r0, 0]
	str r0,[r14, 0]
	add r14,r14,1
	str r0,[r14, 0]
	add r14,r14,1
	str r0,[r14, 0]
	add r14,r14,1
	str r14,[r0, 0]
	mov r14,5
	ldr r13,[r0, 0]
	str r14,[r13, 0]
	add r13,r13,1
	str r13,[r0, 0]
	ldr r14,[r0, 0]
	add r14,r14,-1
	str r14,[r0, 0]
	ldr r13,[r14,0]
	ldr r14, [r0, 1]
	str r13,[r14,0]
	mov r14,4
	ldr r13,[r0, 0]
	str r14,[r13, 0]
	add r13,r13,1
	str r13,[r0, 0]
	ldr r14,[r0, 0]
	add r14,r14,-1
	str r14,[r0, 0]
	ldr r13,[r14,0]
	ldr r14, [r0, 1]
	str r13,[r14,1]
	ldr r13,[r0, 1]
	ldr r14,[r13,0]
	ldr r13,[r0, 0]
	str r14,[r13, 0]
	add r13,r13,1
	str r13,[r0, 0]
	ldr r13,[r0, 1]
	ldr r14,[r13,1]
	ldr r13,[r0, 0]
	str r14,[r13, 0]
	add r13,r13,1
	str r13,[r0, 0]
	ldr r14,[r0, 0]
	add r14,r14,-1
	ldr r13,r14(0)
	add r14,r14,-1
	ldr r12,r14(0)
	add r12,r12,r13
	str r12,[r14,0]
	add r14,r14,1
	str r14,[r0, 0]
	ldr r14,[r0, 0]
	add r14,r14,-1
	str r14,[r0, 0]
	ldr r1,[r14, 0]
	ldr r14,[r0, 0]
	str r1, [r14, 0]
	add r14,r14,1
	str r14,[r0, 0]
	ldr r14,[r0, 0]
	add r14,r14,-1
	str r14,[r0, 0]
	ldr r13,[r14,0]
	ldr r14, [r0, 1]
	str r13,[r14,2]
	ldr r14,[r0, 1]
	ldr r14,[r14,2]
	str r14,[r0,16387]
end:
	b end
