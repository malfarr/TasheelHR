<?php
/**
 * @file
 * Default theme implementation to format an HTML mail.
 *
 * Copy this file in your default theme folder to create a custom themed mail.
 * Rename it to mimemail-message--[module]--[key].tpl.php to override it for a
 * specific mail.
 *
 * Available variables:
 * - $recipient: The recipient of the message
 * - $subject: The message subject
 * - $body: The message body
 * - $css: Internal style sheets
 * - $module: The sending module
 * - $key: The message identifier
 *
 * @see template_preprocess_mimemail_message()
 */
?>
<?php
$header_image_fid = variable_get(MED_VAR_ADMIN_EMAIL_HEADER_IMAGE, '');
$header_imgae_url = med_basic_file_get_managed_file_url($header_image_fid);

$country_name = variable_get(MED_VAR_ADMIN_IMPACT_COUNTRY_NAME, '');

$body_class = '';
if ($module && $key) {
  $body_class = $module . '-' . $key;
}
?>

<?php ?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <title>ProUI Email Template - Welcome</title>
    <meta name="viewport" content="width=device-width" />
    <style type="text/css">
      @media only screen and (max-width: 550px), screen and (max-device-width: 550px) {
        body[yahoo] .buttonwrapper { background-color: transparent !important; }
        body[yahoo] .button { padding: 0 !important; }
        body[yahoo] .button a { background-color: #e67e22; padding: 15px 25px !important; }
      }

      @media only screen and (min-device-width: 601px) {
        .content { width: 600px !important; }
        .col387 { width: 387px !important; }
      }
    </style>
  </head>
  <body id="mimemail-body" bgcolor="#f9f9f9" class="<?php echo $body_class; ?>" style="margin: 0; padding: 0;" yahoo="fix">
    <!--[if (gte mso 9)|(IE)]>
    <table width="600" align="center" cellpadding="0" cellspacing="0" border="0">
      <tr>
        <td>
    <![endif]-->
    <table align="center" border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse; width: 100%; max-width: 600px; border: 1px solid #cccccc; margin-top: 30px;" class="content">
      <tr>
        <td align="center" bgcolor="#ec1c23" style="padding: 10px 20px 10px 20px; color: #ffffff; font-family: Arial, sans-serif; font-size: 36px; font-weight: bold;">
          <img src="<?php echo $header_imgae_url; ?>" alt="MEDAIR" height="70" style="display:block;" />                    
        </td>
      </tr>
      <tr>
        <td bgcolor="#ffffff" style="padding: 20px 20px 20px 20px; color: #555555; font-family: Arial, sans-serif; font-size: 14px; line-height: 16px; border-bottom: 1px solid #f6f6f6;">
          <?php print $body ?>
          <br />
          <br />
          Kind Regards,<br />
          Medair - IMPACT
        </td>
      </tr>

      <tr>
        <td align="center" bgcolor="#dddddd" style="padding: 15px 10px 15px 10px; color: #555555; font-family: Arial, sans-serif; font-size: 12px; line-height: 18px;">
          <b><?php print variable_get(MED_VAR_ADMIN_EMAIL_FOOTER_LINE_1, ''); ?></b><br/><?php print variable_get(MED_VAR_ADMIN_EMAIL_FOOTER_LINE_2, ''); ?><br/>
        </td>
      </tr>
    </table>
    <!--[if (gte mso 9)|(IE)]>
            </td>
        </tr>
    </table>
    <![endif]-->
    <p style="text-align: center; color: #555555;"><?php echo date('Y', time()); ?> &copy; <?php print variable_get(MED_VAR_ADMIN_EMAIL_FOOTER_LINE_3, ''); ?></p>
  </body>
</html>