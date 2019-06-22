<?php
global $base_url;

$project_logo = '';
$international_day_logo = '';

if (isset($variables['project'])) {
  $project_logo_url = med_basic_project_get_pdf_logo_url($variables['project']);
  $project_logo = '<img src="' . $project_logo_url . '" style="max-height: 70px;"/>';
}
if (isset($variables['international_day']) && !empty($variables['international_day'])) {
  $international_day_logo = $variables['international_day'];
}
$third_content = ((isset($variables['third_content']) && !empty($variables['third_content'])) ? $variables['third_content'] : '');

if (isset($variables['projects']) && !empty($variables['projects'])) {
  $projects_logo_url = array();
  foreach ($variables['projects'] as $project_id) {
    $projects_logo_url[] = med_basic_project_get_pdf_logo_url($project_id);
  }
  $logo_width = '100px';
  if (count($projects_logo_url) == 1) {
    $logo_width = '234px';
  }

  $project_logo = '';
  foreach ($projects_logo_url as $project_logo_url) {
    $project_logo .= '<img src="' . $project_logo_url . '" style="margin-left: 5px; max-height: 70px; width: ' . $logo_width . '"/>';
  }
}
?>


<table  border="0" width="100%" style="border: none; vertical-align: top;">
  <tbody>
    <tr>
      <td style="width: 34%; text-align: left;  border: none; vertical-align: top; white-space:nowrap;">
        <table  border="0" width="100%" style="vertical-align: top; border: none;">
          <tr style="vertical-align: top;">
            <td style="text-align: left;  border: none; white-space:nowrap; vertical-align: top;">
              <img src="<?php echo MED_LOGO_MED_PDF ?>" style="max-height: 70px; margin-right: 5px;"/>              
            </td>
            <?php if ($international_day_logo != '') { ?>
              <td style="text-align: left;  border: none; white-space:nowrap;">
                <img src="<?php echo $international_day_logo; ?>" style="max-height: 70px; max-width: 100px;"/>
              </td>
            <?php } ?>
            <?php if (isset($variables['je']) && $variables['je']) { ?>
              <td style="text-align: left;  border: none; white-space:nowrap;">
                <table style=" border: none; height: 30px; font-size: 24pt; text-align: center; margin-left: 10px; margin-top: 10px;">
                  <tr>
                    <td style="border: none; vertical-align: middle;" class="highlight-yellow">JE#</td>
                    <td style="border: none; vertical-align: middle;" class="highlight-yellow">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
                  </tr>
                </table>
              </td>
            <?php } ?>              
          </tr>
        </table>      
      </td>
      <td style="width: 33%; text-align: center; border: none; vertical-align: top;">
        <?php
        if (isset($variables['middle_content'])) {
          echo $variables['middle_content'];
        }
        ?>                  
      </td>            
      <td style="width: 33%; text-align: right;  border: none; vertical-align: top;">
        <table  border="0" width="100%" style="vertical-align: top; border: none;">
          <tr style="vertical-align: top;">
            <?php if ($project_logo != '') { ?>
              <td style="text-align: right;  border: none; white-space:nowrap; vertical-align: top;">
                <?php echo $project_logo; ?>              
              </td>
            <?php } ?>
            
            <?php if ($third_content != '') { ?>
              <td style="text-align: left;  border: none; white-space:nowrap;">
                <?php echo $third_content; ?>
              </td>
            <?php } ?>
          </tr>
        </table>                   
      </td>             
    </tr>
  </tbody>
</table>