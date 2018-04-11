#include "durex.h"
#include "aes.h"

BOOLEAN compare_passwrd(char *no_crypted_passwrd)
{
	char pwd[] = "\\x4c\\x55\\xc8\\x88\\xe1";
	char *passwrd_crypted = gen_passwrd(no_crypted_passwrd);

	if (strlen(passwrd_crypted) != strlen(pwd))
		return (false);
	if (strcmp(passwrd_crypted, pwd) == 0)
		return (true);
	return (false);
}

char	*gen_passwrd(char *key)
{
	t_aes	*aes = NULL;
	char	*result = NULL;

	if ((aes = load_aes()) != NULL) {
		if (get_key_iv(aes) != NULL && init_encryption(aes)
		 && init_decryption(aes))
		{
			t_cipher_plain *cipher_plain = get_cipher_plain();
			 if (cipher_plain) {
				set_plain(cipher_plain, (char*)key, strlen(key));

				encrypt_plain_text(aes, cipher_plain);

				int i = 0;
				char *line = ft_strnew(11);
				while (i < cipher_plain->cipher_len)
				{
					sprintf(line, "\\x%8x", cipher_plain->cipher[i]);
					for (size_t i = 0; i < strlen(line); i++)
						if (line[i] == ' ')
							line[i] = 'f';
					result = ft_dstrjoin(result, ft_dstrjoin("\\x", line + 8, 0), 3);
					bzero(line, 11);
					i++;
				}
				free(line);
			}
		}
	}
	free_aes(aes);
	return (result);
}
