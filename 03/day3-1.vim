" Define the function that will be run when the script is executed
function! FindAndSumCharacters()
  " Initialize the sum to 0
  let sum = 0

  " Iterate over every line in the file
  " Note: The cursor position is not relevant here, since the script
  " will process the entire file
  for line in getline(1, '$')
    " Get the first and second half of the line
    let first_half = line[:len(line)/2]
    let second_half = line[len(line)/2:]

    " Find the character that occurs in both the first and second half
    " of the line.
    " Note: There will be only one such character, since the prompt
    " specified that each line contains only a-zA-Z
    let common_char = ''
    for char in first_half
      if match(second_half, '\C' . char) >= 0
        let common_char = char
        " echo first_half . " - " . second_half . " - char" . common_char
        break
      endif
    endfor

    " Convert the character to a number
    let common_char_num = char2nr(common_char)
    let char_num = 0
    if common_char_num >= char2nr('a') && common_char_num <= char2nr('z')
      let char_num = common_char_num - char2nr('a') + 1
    elseif common_char_num >= char2nr('A') && common_char_num <= char2nr('Z')
      let char_num = common_char_num + 27 - char2nr('A')
    endif
    " echo "char_num " . char_num . "for char " . common_char . " char2nr " . char2nr(common_char)

    " Add the number to the sum
    let sum += char_num
  endfor

  " Output the final sum
  echo sum
endfunction

" Run the script when it is loaded
call FindAndSumCharacters()

