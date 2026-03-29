break Plaintext_read
break _read_pattern_chunk
break _update_sizes
break 289
set args read glider_r.cells
run
display bytes_buffer
display c
display stmt->state
set print pretty on
